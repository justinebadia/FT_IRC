#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "color.hpp"


using namespace irc;

void	CommandManager::cmd_user( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	if(msg.get_param_count() < 4)
		return (run_reply(ERR_NEEDMOREPARAMS, msg));
	if(!client.get_username().empty())
		return(run_reply(ERR_ALREADYREGISTERED, msg));
	if (!msg[1].empty())
		client.set_username(msg[1]);
	if ( !msg.get_substr_after(":").empty() )
		client.set_realname(msg.get_substr_after(":"));
	if (msg[3].length() == 1 && msg[3][0] == '*')
		client.set_hostname(client.get_client_ip());
	else
		client.set_hostname(msg[3]);
	client.set_registration_flags(Client::USER_SET);
	Server::log("Successfully set the username to " + msg.get_substr_after(":") + RESET);
	return ;
}
