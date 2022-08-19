#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"

using namespace irc;

void	CommandManager::cmd_oper( Message& msg )
{
	int	reply;
	if(msg.get_param_count() < 2)
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	reply = _server->attempt_client_as_operator(*msg.get_client_ptr(), msg[1], msg[2]);
	if (reply != 0)
		return run_reply(reply, msg);
	
}
