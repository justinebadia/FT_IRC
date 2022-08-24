#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "Channel.hpp"


using namespace irc;
using std::cout;
using std::cerr;
using std::endl;

void CommandManager::process_single_kick( Message& msg )
{
	Client*				source_client = msg.get_client_ptr();
	Client*				target_client;
	t_client_ptr_list	recipient_list;
	string				user_param = msg[2];
	size_t				next_pos = 0;
	size_t				pos = 0;

	Channel* channel = _database->get_channel(msg[1]);
	if (!channel)
	{
		run_reply(ERR_NOSUCHCHANNEL, msg);
		return ;
	}
	if (msg[1][0] != '&' && msg[1][0] != '#')
	{
		run_reply(ERR_BADCHANMASK, msg); 
		return;
	}
	if (channel->is_member(source_client) == false)
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}
	if (!channel->is_owner(source_client) && !channel->is_chanop(source_client))
	{	
		run_reply(ERR_CHANOPRIVSNEEDED, msg);
		return;
	}
	//MULTIPLE USER KICK LOOP
	while (pos < user_param.length())
	{
		if (user_param[pos] == ',')
		{
			pos++;
			continue;
		}
		next_pos = user_param.find(',', pos);
		if (next_pos == string::npos)
			next_pos = user_param.length();
		target_client = _database->get_client(user_param.substr(pos, next_pos - pos));
		if (!target_client || !channel->is_member(target_client))
		{
			run_reply(ERR_USERNOTINCHANNEL, msg);
		}
		else
		{
			recipient_list = channel->get_clients_not_banned();
			if (msg[3].empty() == false)
			{
				send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "KICK " + msg[1] + " " + target_client->get_nickname() + " :" + msg.get_substr_after(":") + CRLF);
			}
			else
			{
				send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "KICK " + msg[1] + " " + target_client->get_nickname()  + " :No reason given" + CRLF);
			}
			channel->remove_member(target_client);
		}
		pos = next_pos + 1;
	}
}

void CommandManager::cmd_kick( Message& msg )			
{
	size_t		next_pos = 0;
	size_t		pos = 0;
	string		channel_param;
	string		user_param;
	Message		single_kick_msg(msg.get_client_ptr());
	list<string>	user_to_kick;
	list<string>::iterator it;

	if (msg.get_param_count() < 2)
	{
		return run_reply(ERR_NEEDMOREPARAMS, msg);
	}
	channel_param = msg[1];
	user_param = msg[2];
	if (count_elements_in_param(channel_param) == 1)
	{
		return process_single_kick(msg);
	}

	if (count_elements_in_param(channel_param) > 1 && count_elements_in_param(channel_param) != count_elements_in_param(user_param))
	{
		return run_reply(ERR_NEEDMOREPARAMS, msg);
	}


	while (pos < user_param.length())
	{
		if (user_param[pos] == ',')
		{
			pos++;
			continue;
		}
		next_pos = user_param.find(',', pos);
		if (next_pos == string::npos)
			next_pos = user_param.length();
		user_to_kick.push_back(user_param.substr(pos, next_pos - pos));
		pos = next_pos + 1;
	}

	it = user_to_kick.begin();
	pos = 0;
	next_pos = 0;
	while (pos < channel_param.length())
	{
		if (channel_param[pos] == ',')
		{
			pos++;
			continue;
		}
		next_pos = channel_param.find(',', pos);
		if (next_pos == string::npos)
			next_pos = channel_param.length();
		if (!msg.get_substr_after(":").empty())
			single_kick_msg.append_in("KICK " + channel_param.substr(pos, next_pos - pos) + " " + *it + " :" + msg.get_substr_after(":"));
		else
			single_kick_msg.append_in("KICK " + channel_param.substr(pos, next_pos - pos) + " " + *it);

		process_single_kick(single_kick_msg);

		msg.append_out(single_kick_msg.get_message_out());
		single_kick_msg.clear_all();
		pos = next_pos + 1;
		it++;
	}
}
