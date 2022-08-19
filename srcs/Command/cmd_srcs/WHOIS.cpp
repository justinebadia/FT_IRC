#include "CommandManager.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"

using namespace irc;

void CommandManager::cmd_whois( Message & msg )
{
	size_t pos = 0;
	size_t first = 0;
	string nick_to_test;
	string nick_list;
	t_channel_ptr_list channels;

	if (msg.get_param_count() > 2)
	{
		if ( msg[1] != _server->get_server_ip())
		{
			run_reply(ERR_NOSUCHSERVER, msg);
			return ;
		}
		nick_list = msg[2];
	}
	else
		nick_list = msg[1];
	while (pos < nick_list.length())
	{
		pos = nick_list.find(",", first);
		nick_to_test = nick_list.substr(first, pos);
		Client* client = _database->get_client(nick_to_test);			
		if (client)
		{
			run_reply(RPL_WHOISUSER, msg);
			run_reply(RPL_WHOISSERVER, msg);
			if (client->is_operator())
				run_reply(RPL_WHOISOPERATOR, msg);
			channels = _database->get_channel_list_of_client(client);
			if (channels.size() > 0)
				run_reply(RPL_WHOISCHANNELS, msg);
			run_reply(RPL_ENDOFWHOIS, msg);

		}
		first = pos + 1;
	}
	return;
}
