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

void CommandManager::cmd_error( Message& msg )
{
	Server::log("Error received from " + msg.get_client_ptr()->get_nickname() + ", the message is: " + msg.get_substr_after(";"));
	return;
}
