#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "Channel.hpp"


using namespace irc;

void    CommandManager::process_single_part( Message& msg )
{
	Client*				source_client = msg.get_client_ptr();	
	Channel*			channel = _database->get_channel(msg[1]);
	string				reason;
	
	if (!channel)
	{
		run_reply(ERR_NOSUCHCHANNEL, msg);
	}
	else if (channel->is_member(source_client) == false)		// if the source is not a member of the channel
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
	}
	else
	{
		t_client_ptr_list	recipient_list;
		// recipient_list = channel->get_clients_not_matching_permissions(BAN); WARNING
		recipient_list = channel->get_clients_not_banned();
		reason = msg.get_substr_after(":");
		if (reason.empty())
			reason = "No reason given";
		send_to_clients(recipient_list, source_client->get_prefix() + " PART " + channel->get_name() + " :" + reason + CRLF);
		channel->remove_member(source_client);
	}
}

void	CommandManager::cmd_part( Message& msg )
{
	size_t		next_pos = 0;
	size_t		pos = 0;
	string		channels;
	string		chan_name;
	string		reason;
	Message		single_join_msg(msg.get_client_ptr());

	channels = msg[1];
	if (channels.empty())
	{
		return run_reply(ERR_NEEDMOREPARAMS, msg);
	}
	reason = msg.get_substr_after(":");
	if (!reason.empty())
		reason.insert(0, " :");
	while (pos < channels.length())
	{
		if (channels[pos] == ',')
		{
			pos++;
			continue;
		}
		next_pos = channels.find(',', pos);
		if (next_pos == string::npos)
			next_pos = channels.length();

		single_join_msg.append_in("PART " + channels.substr(pos, next_pos - pos) + reason);
		process_single_part(single_join_msg);
		msg.append_out(single_join_msg.get_message_out());
		single_join_msg.clear_all();
		pos = next_pos + 1;
	}
}
