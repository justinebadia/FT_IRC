#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"


using namespace irc;


void CommandManager::cmd_ping( Message& msg )
{
	//if (!msg[1].empty() && msg[1] != _server->get_server_ip() && !_database->get_client(msg[1]))
	//run_reply(ERR_NOSUCHSERVER, msg);
	//else
	msg.append_out("PONG :" + _server->get_server_ip() + CRLF);
		
	return;
}
