#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include <vector>

using namespace irc;

using std::cerr;
using std::cout;
using std::endl;


vector<string> tokenize_params(string const& str)
{
	vector<string> res;

	char* token = std::strtok(const_cast<char*>(str.c_str()), " ");
	while (token)
	{
		res.push_back(string(token));
		token = strtok(nullptr, " ");
	}
	return res;
}

void CommandManager::handle_o_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel *channel, Message& msg)
{
	pos_it++;
	Client *to_chanop = _database->get_client(parsed[pos_it]);
	if (to_chanop)
	{
		
		if (channel->is_member(to_chanop))
		{
			if (msg[2][0] == '-')
				channel->set_permission(to_chanop, REGULAR);
			else
				channel->set_permission(to_chanop, CHANOP);
			modes += "o";
			params += parsed[pos_it];
		}
		else
		{
			irc::CommandManager::run_reply(ERR_NOTONCHANNEL, msg);
			return;
		}
	}
	return;
}

void CommandManager::handle_k_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg)
{
	pos_it++;
	if (!channel->get_password().empty())
	{
		run_reply(ERR_KEYSET, msg);
		return;
	}
	if (msg[2][0] == '-')
		modes += "k";
	else if (!parsed[pos_it].empty())
	{
		channel->set_password(parsed[pos_it]);
		modes += "k";
		channel->set_mode_str("k");
		params += parsed[pos_it];
	}
	return;
}

void CommandManager::handle_b_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg)
{
	pos_it++;
	if (pos_it >= parsed.size())
	{
		irc::CommandManager::run_reply(RPL_BANLIST, msg);
		irc::CommandManager::run_reply(RPL_ENDOFBANLIST, msg);
		return;
	}
	else
	{
		if (msg[2][0] == '-')
			channel->remove_banmask(parsed[pos_it]);
		else if (msg[2][0] == '+')
			channel->add_banmask(parsed[pos_it]);
		modes += "b";
		params += parsed[pos_it];
	}
	return;
}


void CommandManager::cmd_mode(Message &msg)
{
	string				modes = "";
	string				params = "";
	size_t				pos_it = 0;
	Client*				source_client = msg.get_client_ptr();
	t_client_ptr_list	recipient_list;
	/*Checking for channel's errors*/
	if (msg.get_param_count() < 1)
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		Channel *channel = _database->get_channel(msg[1]);
		if (!channel)
		{
			run_reply(ERR_NOSUCHCHANNEL, msg);
			return;
		}
		if (msg.get_param_count() == 1) // j'affiche les modes du channel
		{
			run_reply(RPL_CHANNELMODEIS, msg);
			return;
		}

		/*if there are more than 2 parameters, it means there are flags, so the client need to be chanop/owner */
		if (msg.get_param_count() >= 2)
		{
			Client *client = msg.get_client_ptr();
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
		}

		/*Parsing the string*/
		vector<string> parsed = tokenize_params(channel->parse_modes(msg)); // tokenize the flags and their parameters
		if (channel->get_mode_flags() > 0)
		{
			for (size_t i = 0; i < parsed[0].length(); i++)
			{
				if (parsed[0][i] == 'o')
					handle_o_mode(pos_it, modes, params, parsed, channel, msg);
				else if (parsed[0][i] == 'i')
				{
					modes += "i";
					channel->set_mode_str("i");
				}
				else if (parsed[0][i] == 't')
				{
					modes += "t";
					channel->set_mode_str("t");
				}
				else if (parsed[0][i] == 'k')
					handle_k_mode(pos_it, modes, params, parsed, channel, msg);
				else if (parsed[0][i] == 'b')
					handle_b_mode(pos_it, modes, params, parsed, channel, msg);
			}
			msg.set_mode_rpl(msg[2][0] + modes + " " + params);
		}
		run_reply(RPL_CHANNELMODEIS, msg);
		recipient_list = channel->get_clients_not_matching_permissions(BAN);
		send_to_clients(recipient_list, source_client->get_prefix() + "MODE " + msg[1] + " " + msg[2] + " " + msg[3] + CRLF);
		
	}
	return;
}
