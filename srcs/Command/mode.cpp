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

map<string, int> flags_need_params( string to_parse )
{
	int pos = 1;
	map<string, int> pos_param;

	for (size_t i = 0; i != to_parse.length(); i++)
	{
		if (to_parse[i] == 'o')
		{
			pos_param.insert(std::make_pair("o", pos));
			pos++;
		}
		if (to_parse[i] == 'k')
		{
			pos_param.insert(std::make_pair("k", pos));
			pos++;
		}
		if (to_parse[i] == 'b')
		{
			pos_param.insert(std::make_pair("b", pos));
			pos++;
		}
	}
	return pos_param;
}

int	find_param_pos(map<string, int> pos_param, string to_find)
{
	map<string, int>::iterator it = pos_param.begin();
	map<string, int>::iterator ite = pos_param.end();

	for(; it != ite; it++)
	{
		if ((*it).first == to_find)
			return (*it).second;
	}
	return -1;
}

void CommandManager::handle_o_mode(string& modes, string& params, std::vector<string> parsed, Channel *channel, map<string, int> pos_param, Message& msg)
{
	size_t pos = static_cast<size_t>(find_param_pos(pos_param, "o"));
	Client *to_chanop = _database->get_client(parsed[pos]);
	if (find_param_pos(pos_param, "o"))
	{
		if (to_chanop)
		{
			if (channel->is_member(to_chanop))
			{
				channel->set_permission(to_chanop, CHANOP);
				modes += "o";
				params += parsed[pos];
			}
			else
			{
				irc::CommandManager::run_reply(ERR_NOTONCHANNEL, msg);
				return;
			}
		}
	}
	return;
}

void CommandManager::handle_k_mode(string& modes, string& params, std::vector<string> parsed, Channel* channel, map<string, int> pos_param, Message& msg)
{
	size_t pos = static_cast<size_t>(find_param_pos(pos_param, "k"));
	if (!channel->get_password().empty())
	{
		run_reply(ERR_KEYSET, msg);
		return;
	}
	else if (find_param_pos(pos_param, "k"))
	{
		if (!parsed[pos].empty())
		{
			channel->set_password(parsed[pos]);
			modes += "k";
			params += parsed[pos];
		}
	}
	return;
}

void CommandManager::handle_b_mode(string& modes, string& params, std::vector<string> parsed, map<string, int> pos_param, Message& msg)
{
	size_t pos = static_cast<size_t>(find_param_pos(pos_param, "b"));
	if (find_param_pos(pos_param, "b"))
	{
		if (parsed[pos].empty())
		{
			irc::CommandManager::run_reply(RPL_BANLIST, msg);
			irc::CommandManager::run_reply(RPL_ENDOFBANLIST, msg);
			return;
		}
		else
		{
			modes += "b";
			params += parsed[pos];
		}
	}
	return;
}


void CommandManager::cmd_mode(Message &msg)
{
	string modes = "";
	string params = "";
	/*Checking for channel's errors*/
	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		if (msg.get_param_count() < 1)
		{
			run_reply(ERR_NEEDMOREPARAMS, msg);
			return ;
		}
		Channel *channel = _database->get_channel(msg[1]);
		if (!channel)
		{
			run_reply(ERR_NOSUCHCHANNEL, msg);
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
	/*Parsing the string*/
			//string to_parse = channel->parse_modes(msg); //return the substring after + or -
			vector<string> parsed = tokenize_params(channel->parse_modes(msg));	// tokenize the flags and their parameters
			map<string, int> pos_param = flags_need_params(parsed[0]);
			if (channel->get_mode_flags() > 0)
			{
				for (size_t i = 0; i != parsed[0].length(); i++)
				{
					if (parsed[0][i] == 'o')
						handle_o_mode(modes, params, parsed, channel, pos_param, msg);
					else if (parsed[0][i] == 'i')
						modes += "i";
					else if (parsed[0][i] == 't')
						modes += "t";
					else if (parsed[0][i] == 'k')
						handle_k_mode(modes, params, parsed, channel, pos_param, msg);
					else if (parsed[0][i] == 'b')
						handle_b_mode(modes, params, parsed, pos_param, msg);
				}
			}
			msg.set_mode_rpl(msg[2][0] + modes + " " + params);
		}
	irc::CommandManager::run_reply(RPL_CHANNELMODEIS, msg);
	}
	return ;
}
