#include "CommandManager.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"

using namespace irc;

void CommandManager::cmd_squit( Message& msg )
{
	Client*				client	= msg.get_client_ptr();

	if (_server->is_client_operator(client))
	{
		_server->set_exit(true);
	}
}
