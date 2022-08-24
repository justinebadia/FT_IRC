#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "color.hpp"

using namespace irc;

void	CommandManager::cmd_nick( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();
	
	if (msg.get_param_count() == 0)
		return run_reply(ERR_NONICKNAMEGIVEN, msg);
	if ( !validate_with_regex(REGEX_NICKNAME, msg[1]) )
	{
		run_reply(ERR_ERRONEUSNICKNAME, msg);
		return;
	}
	if ( _database->get_client( msg[1] ) )
	{
		run_reply(ERR_NICKNAMEINUSE, msg);
		return;
	}
	client.set_registration_flags(Client::NICK_SET);
	Server::log("Successfully set the nickname to " + msg[1] + RESET);
	client.append_buff(BUFFOUT, client.get_prefix()+ "NICK " + msg[1] + CRLF);
	client.set_nickname(msg[1]);
}
