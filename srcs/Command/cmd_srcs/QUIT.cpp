#include "CommandManager.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"

using namespace irc;

void CommandManager::cmd_quit( Message& msg )
{
	Client*				client	= msg.get_client_ptr();
	string				prefix = client->get_prefix();
	t_channel_ptr_list	chan_list = _database->get_channel_list_of_client(client);

	if (!client)
		return;
	client->set_to_be_killed(true);
	msg.set_client_ptr(NULL);
	if (!msg.get_substr_after(":").empty())
		send_to_channels(chan_list, prefix + "QUIT :Quit: " + msg.get_substr_after(":") + CRLF);	
	if (msg[1].empty())
		send_to_channels(chan_list, prefix + "QUIT :Quit: No reason given" + CRLF);
	msg.append_out("Error :connection will be closed..." + CRLF);
}
