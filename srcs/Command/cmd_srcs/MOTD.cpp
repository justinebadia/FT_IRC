#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "Channel.hpp"

using namespace irc;

void CommandManager::cmd_motd( Message& msg )
{
	run_reply(RPL_MOTDSTART, msg);
	run_reply(RPL_MOTD, msg);
	run_reply(RPL_ENDOFMOTD, msg);
}
