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

void CommandManager::process_single_join( Message& msg )
{
	t_client_ptr_list	client_list;
	Channel* 			channel;
	string topic;
	Client* client_ptr = msg.get_client_ptr();

	if (_database->get_channel_count() >= MAX_CHANNELS)
	{
		return run_reply(ERR_TOOMANYCHANNELS, msg);
	}
	if (!validate_entry(REGEX_CHANNEL, msg[1]))
	{
		return run_reply(ERR_BADCHANMASK, msg);
	}
	channel = _database->get_channel(msg[1]);
	if (!channel) // if channel doesn't exist : create a new one
	{
		_database->add_channel_list(Channel(string(msg[1]), msg.get_client_ptr()));
		channel = _database->get_channel(msg[1]);
	}
	else if (channel->get_memberlist().size() >= MAX_CLIENT_PER_CHAN)
	{
		return run_reply(ERR_CHANNELISFULL, msg);
	}
	else if (channel->is_banned(client_ptr))
	{
		return run_reply(ERR_BANNEDFROMCHAN, msg);
	}
	else if (channel->get_is_invite_only())
	{
		// if (INVITED == true)
		// {	
		// 	INVITED = false;
		// 	channel->add_member(client_ptr, REGULAR);
		 
		// }
		// else
		
			
			return run_reply(ERR_INVITEONLYCHAN, msg);
	}
	if (!channel->is_member(client_ptr))
	{
		channel->add_member(client_ptr, REGULAR);
		topic = channel->get_topic();
		if (topic.size() == 0)				// WARNING A VERIFIER si NULL ou string vide ""
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
	}
	// WARNING ERR_NOSUCHCHANNEL ???
	client_list = channel->get_clients_not_matching_permissions(BAN);
	send_to_clients(client_list, client_ptr->get_prefix() + "JOIN " + msg[1] + CRLF);
}

void CommandManager::cmd_join( Message& msg )			
{
	size_t		next_pos = 0;
	size_t		pos = 0;
	string		channels;
	string		chan_name;
	Message		single_join_msg(msg.get_client_ptr());

	channels = msg[1];
	if (channels.empty())
	{
		return run_reply(ERR_NEEDMOREPARAMS, msg);
	}
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
		single_join_msg.append_in("JOIN " + channels.substr(pos, next_pos - pos));
		process_single_join(single_join_msg);
		msg.append_out(single_join_msg.get_message_out());
		single_join_msg.clear_all();
		pos = next_pos + 1;
	}
	return ;
}
