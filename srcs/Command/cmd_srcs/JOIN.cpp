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
				Server::log("An invite coupon has been consumed");
			}
			else
			{
				return run_reply(ERR_INVITEONLYCHAN, msg);
			}
		}
		else if (channel->is_banned(source_client))
		{
			return run_reply(ERR_BANNEDFROMCHAN, msg);
		}
		if (channel->get_is_password_required() == true)
		{
			Server::log(channel->get_password() + " " + msg[2]);
			if (channel->get_password().compare(msg[2]) )
				return run_reply(ERR_BADCHANNELKEY, msg);
		}
		channel->add_member(source_client, REGULAR);
		topic = channel->get_topic();
		if (topic.empty() == true)
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
		recipient_list = channel->get_clients_not_banned();
		recipient_list.remove(source_client);
		recipient_list.push_back(source_client);
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
		recipient_list = channel->get_clients_not_banned();
		send_to_clients(recipient_list, source_client->get_prefix() + "JOIN " + msg[1] + CRLF);
	}
	Message	msg_names(source_client, "NAMES " + msg[1]);
	cmd_names(msg_names);
	msg.append_out(msg_names.get_message_out());

}

void CommandManager::cmd_join( Message& msg )			
{
	size_t		next_pos = 0;
	size_t		pos = 0;
	string		channels;
	string		chan_name;
	size_t		key_pos = 0;
	size_t		key_next_pos = 0;
	string		keys;
	string		key;
	Message		single_join_msg(msg.get_client_ptr());

	channels = msg[1];
	keys = msg[2];

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
		if (keys[key_pos] == ',')
		{
			key_pos++;
			continue;
		}

		next_pos = channels.find(',', pos);
		if (next_pos == string::npos)
			next_pos = channels.length();

		key_next_pos = keys.find(',', pos);
		if (key_next_pos == string::npos)
			key_next_pos = keys.length();

		if (keys.empty() || key_pos > keys.length())
			single_join_msg.append_in("JOIN " + channels.substr(pos, next_pos - pos));
		else
			single_join_msg.append_in("JOIN " + channels.substr(pos, next_pos - pos) + " " + keys.substr(key_pos, key_next_pos - key_pos));

		process_single_join(single_join_msg);
		msg.append_out(single_join_msg.get_message_out());
		Server::log(msg.get_message_out());
		single_join_msg.clear_all();
		pos = next_pos + 1;
		key_pos = key_next_pos + 1;
	}
	return ;
}
