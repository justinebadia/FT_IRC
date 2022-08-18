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
	Client* source_client = msg.get_client_ptr();
	t_client_ptr_list	recipient_list;
	Channel* 			channel = NULL;
	string topic;

	// JOIN ZERO
	if (msg[1] == "0")
	{
		// source_client PART all joined channels
		t_channel_ptr_list joined_channel_list = _database->get_channel_list_of_client(source_client);
		t_channel_ptr_list::iterator it = joined_channel_list.begin();
		t_channel_ptr_list::iterator ite = joined_channel_list.end();

		for (; it != ite; it++)
		{
			recipient_list = channel->get_clients_any_permissions();
			send_to_clients(recipient_list, source_client->get_prefix() + " PART " + channel->get_name() + CRLF);
			(*it)->remove_member(source_client);
		}
		return;
	}
	channel = _database->get_channel(msg[1]);
	if (!validate_with_regex(REGEX_CHANNEL, msg[1]))
	{
		return run_reply(ERR_BADCHANMASK, msg);
	}
	if (_database->is_channel_listed(msg[1]) == true)
	{
		if (channel->get_memberlist().size() >= MAX_CLIENT_PER_CHAN)
		{
			return run_reply(ERR_CHANNELISFULL, msg);
		}
		if (channel->get_is_invite_only())
		{
			if (_database->use_invite_coupon(source_client, channel) == SUCCESS)
			{
				// cout << "found the invite coupon!" << endl;
			}
			else
			{
				return run_reply(ERR_INVITEONLYCHAN, msg);
			}
		}
		else if (channel->is_banned(source_client))// WARNING Ne fonctionne plus avec le banmask
		{
			return run_reply(ERR_BANNEDFROMCHAN, msg);
		}
		if (channel->get_is_password_required() == true)
		{
			if (channel->get_password() != msg[2])
				return run_reply(ERR_BADCHANNELKEY, msg);
		}
		channel->add_member(source_client, OWNER);
		topic = channel->get_topic();
		if (topic.empty() == true)
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
		recipient_list = channel->get_clients_not_matching_permissions(BAN);
		send_to_clients(recipient_list, source_client->get_prefix() + "JOIN " + msg[1] + CRLF);
	}
	else	// the channel doesn't exist
	{
		if (_database->get_channel_count() >= MAX_CHANNELS)
		{
			return run_reply(ERR_TOOMANYCHANNELS, msg);
		}	
		_database->add_channel_list(Channel(string(msg[1]), msg.get_client_ptr()));
		channel = _database->get_channel(msg[1]);
		channel->add_member(source_client, OWNER);
		topic = channel->get_topic();
		if (topic.empty() == true)
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
		recipient_list = channel->get_clients_not_matching_permissions(BAN);
		send_to_clients(recipient_list, source_client->get_prefix() + "JOIN " + msg[1] + CRLF);
	}
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
