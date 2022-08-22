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
	vector<string> 	res;
	size_t			next_pos = 0;
	size_t			pos = 0;
	
	while (pos < str.length())
	{
		if (str[pos] == ' ')
		{
			pos++;
			continue;
		}
		next_pos = str.find(' ', pos);
		if (next_pos == string::npos)
			next_pos = str.length();
		res.push_back(str.substr(pos, next_pos - pos));
		pos = next_pos + 1;
	}
	return res;
}

void CommandManager::handle_o_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel *channel, Message& msg)
{
	
	Client *target_client;
	pos_it++;
	if (pos_it >= parsed.size() || parsed[pos_it].empty())
		return ;
	target_client = _database->get_client(parsed[pos_it]);
	if (target_client)
	{
		if (channel->is_member(target_client))
		{
			if (msg[2][0] == '-')
			{
				if (channel->is_owner(target_client))
					channel->set_channel_owner(NULL);
				channel->set_permission(target_client, REGULAR);
			}
			else
				channel->set_permission(target_client, CHANOP);
			modes += "o";
			params += " " + parsed[pos_it];
		}
		else
		{
			run_reply(ERR_USERNOTINCHANNEL, msg);
			return;
		}
	}
	return;
}

void CommandManager::handle_k_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg)
{
	if (msg[2][0] == '-')
		modes += "k";
	else 
	{
		pos_it++;
		if (!channel->get_password().empty())
		{
			run_reply(ERR_KEYSET, msg);
			return;
		}
		else if (pos_it < parsed.size() && !parsed[pos_it].empty())
		{
			channel->set_password(parsed[pos_it]);
			modes += "k";
			channel->set_mode_str("k");
			params += " " + parsed[pos_it];
		}
	}
	return;
}

void CommandManager::handle_b_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg)
{
	pos_it++;
	if (pos_it >= parsed.size())
	{
		run_reply(RPL_BANLIST, msg);
		run_reply(RPL_ENDOFBANLIST, msg);
		return;
	}
	else if (pos_it < parsed.size() && !parsed[pos_it].empty())
	{
		if (msg[2][0] == '-')
			channel->remove_banmask(parsed[pos_it]);
		else
			channel->add_banmask(parsed[pos_it]);
		modes += "b";
		params += " " + parsed[pos_it];
	}
	return;
}


void CommandManager::cmd_mode(Message &msg)
{
	string				modes = "";
	string				params = "";
	string 				format = "";
	size_t				pos_it = 0;
	Client*				source_client = msg.get_client_ptr();
	t_client_ptr_list	recipient_list;
	Channel* 			channel;
	/*Checking for channel's errors*/
	if (msg.get_param_count() < 1)
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		channel = _database->get_channel(msg[1]);
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
			if (!(channel->is_chanop(source_client)) && !(channel->is_owner(source_client)))
			{
				run_reply(ERR_CHANOPRIVSNEEDED, msg);
				return;
			}
			if (!channel->is_member(source_client))
			{
				run_reply(ERR_NOTONCHANNEL, msg);
				return;
			}
		}

		/*Parsing the string*/
		vector<string> parsed = tokenize_params(channel->parse_modes(msg)); // tokenize the flags and their parameters
		if (channel->get_mode_flags() > 0 && !parsed.empty())
		{
			for (size_t i = 0; i < parsed[0].length(); i++)
			{
				msg.set_special_output(string(1, parsed[0][i]));
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
				else
					run_reply(ERR_UNKNOWNMODE, msg);
			}
			if(msg[2][0] == '-')
				msg.set_mode_rpl(msg[2][0] + modes + params);
			else
				msg.set_mode_rpl("+" + modes + params);
		}
		run_reply(RPL_CHANNELMODEIS, msg);
		recipient_list = channel->get_clients_not_banned();
		recipient_list.remove(source_client);
		if (!modes.empty())
		{
			format = source_client->get_prefix() + "MODE " + msg[1] + " " + modes;
			if (!params.empty())
				format += params;
			send_to_clients(recipient_list, format + CRLF);
		}
		
	}
	return;
}
