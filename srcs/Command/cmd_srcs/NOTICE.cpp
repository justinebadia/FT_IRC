#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"


using namespace irc;

void	CommandManager::cmd_notice( Message& msg )
{ 
	t_client_ptr_list		recipient_list;
	Channel* 				channel;
	Client*					client;
	string					prefix;

	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		channel = _database->get_channel(msg[1]);
		if (channel)
		{
			client = msg.get_client_ptr();
			recipient_list = channel->get_clients_not_matching_permissions(BAN);
			recipient_list.remove(client);
			send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "NOTICE " + channel->get_name() + " " + msg.get_substr_after(":") + CRLF);
		}
	}
	else
	{
		client = _database->get_client(msg[1]);
		if (client)
			client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "NOTICE " + msg[1] + " " + msg.get_substr_after(":") + CRLF);
	}
	return;
	
}
