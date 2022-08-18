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

void CommandManager::process_single_names( Message& msg )
{
	Client* source_client = msg.get_client_ptr();
	t_client_ptr_list	recipient_list;
	Channel* 			channel = NULL;
	string topic;

	run_reply(RPL_NAMREPLY, msg);
	run_reply(RPL_ENDOFNAMES, msg);
	recipient_list.push_back(source_client);
	send_to_clients(recipient_list, source_client->get_prefix() + "NAMES " + msg[1] + CRLF);
	
}

void CommandManager::cmd_names( Message& msg )			
{
	size_t		next_pos = 0;
	size_t		pos = 0;
	string		channels;
	string		chan_name;
	Message		single_join_msg(msg.get_client_ptr());

	channels = msg[1];
	if (channels.empty())	//make a string of all channels
	{
		t_channel_list channel_list = _database->get_channel_list();
		t_channel_list::iterator it = channel_list.begin();
		t_channel_list::iterator ite = channel_list.end();

		for (; it != ite; it++)
		{
			channels += (*it).get_name();
			channels += ",";
		}
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
		single_join_msg.append_in("NAMES " + channels.substr(pos, next_pos - pos));
		process_single_names(single_join_msg);
		msg.append_out(single_join_msg.get_message_out());
		single_join_msg.clear_all();
		pos = next_pos + 1;
	}
	return ;
}
