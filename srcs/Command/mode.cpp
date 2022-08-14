#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "Channel.hpp"

using namespace irc;

using std::cerr;
using std::cout;
using std::endl;

int parse_cmd_mode(Message &msg)
{
	Client *client = msg.get_client_ptr();

	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		if (msg.get_param_count() < 1)
		{
			irc::CommandManager::run_reply(ERR_NEEDMOREPARAMS, msg);
			return -1;
		}
		return 1; // 1 = mode channel
	}
	else
		return 0; // mode user
}

void check_chan_mode_3args( irc::Channel *channel, Message &msg)
{
	switch (channel->get_mode_flags())
	{
	case irc::Channel::FLAG_B:
	{
		irc::CommandManager::run_reply(RPL_BANLIST, msg);
		irc::CommandManager::run_reply(RPL_ENDOFBANLIST, msg);
		break;
	}
	default:
		irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
		break;
	}
}

void check_chan_mode_more(irc::Channel *channel, Message &msg, Client *target3, Client *target4)
{
	switch (channel->get_mode_flags())
	{
		case irc::Channel::FLAG_O:
		{
			irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
			if (target3)
			{
				if (channel->is_member(target3))
					channel->set_permission(target3, CHANOP);
				else
					irc::CommandManager::run_reply(ERR_NOTONCHANNEL, msg);
			}
			break;
		}
		case (irc::Channel::FLAG_I + irc::Channel::FLAG_T + irc::Channel::FLAG_K):
		case (irc::Channel::FLAG_I + irc::Channel::FLAG_K):
		case (irc::Channel::FLAG_T + irc::Channel::FLAG_K):
		case (irc::Channel::FLAG_K):
		{
			irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
			if (!msg[3].empty())
				channel->set_password((msg[3]));
			break;
		}
		case irc::Channel::FLAG_B: // modifier cmd_join et cmd_privmsg
		{
			if (target3)
				channel->set_permission(target3, BAN);
			break;
		}
		case (irc::Channel::FLAG_K +irc::Channel::FLAG_O):
		case (irc::Channel::FLAG_K + irc::Channel::FLAG_O + irc::Channel::FLAG_T):
		{
			irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
			if (msg[2][1] == 'k' && msg[2][2] == 'o')
			{
				if (!msg[3].empty())
					channel->set_password((msg[3]));
				if (target4)
				{
					if (channel->is_member(target4))
						channel->set_permission(target4, CHANOP);
					else
						irc::CommandManager::run_reply(ERR_NOTONCHANNEL, msg); // si l'argument du flag est incorrect, irssi ne renvoie pas d'erreur, je ne sais pas si on doit le faire ou pas
				}
			}
			if (msg[2][1] == 'o' && msg[2][2] == 'k')
			{
				if (!msg[4].empty())
					channel->set_password((msg[4]));
				if (target3)
				{
					if (channel->is_member(target3))
						channel->set_permission(target3, CHANOP);
					else
						irc::CommandManager::run_reply(ERR_NOTONCHANNEL, msg);
				}
			}
			break;
		}
		case (irc::Channel::FLAG_K + irc::Channel::FLAG_B):
		{
			irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
			if (msg[2][1] == 'k' && msg[2][2] == 'b')
			{
				if (!msg[3].empty())
					channel->set_password((msg[3]));
				if (target4)
					channel->set_permission(target4, BAN);
			}
			if (msg[2][1] == 'b' && msg[2][2] == 'k')
			{
				if (!msg[4].empty())
					channel->set_password((msg[4]));
				if (target3)
					channel->set_permission(target3, BAN);
			}
			break;
		}
	}
}

void CommandManager::cmd_mode(Message &msg) // attention les yeux - faire une fonction qui parse la commande
{
	t_channel_ptr_list chan_list;
	Client *client = msg.get_client_ptr();

	std::cout << msg.get_param_count() << endl;
	if (parse_cmd_mode(msg) == 1)
	{
		Channel *channel = _database->get_channel(msg[1]);
		if (!channel)
		{
			run_reply(ERR_NOSUCHCHANNEL, msg);
			return;
		}
		if (!(channel->is_chanop(client)) && !(channel->is_owner(client)))
		{
			run_reply(ERR_CHANOPRIVSNEEDED, msg);
			return;
		}
		if (!channel->is_member(client))
		{
			run_reply(ERR_NOTONCHANNEL, msg);
			return;
		}
		if (channel->parse_modes(msg) > 0) // vérifier si des flags sont actifs
		{
			if (msg.get_param_count() < 3) // commande possible sans nickname +i +t +b
				check_chan_mode_3args(channel, msg);
			else // command ou nickname est obligatoire +o +b +k
			{
				Client *target3 = _database->get_client(msg[3]);
				Client *target4 = _database->get_client(msg[4]);
				check_chan_mode_more(channel, msg, target3, target4);
			}
		}
	}
	else if (parse_cmd_mode(msg) == 0) // MODE user case
	{
		return;
	}
}
