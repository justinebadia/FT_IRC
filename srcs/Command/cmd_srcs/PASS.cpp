#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"


using namespace irc;

void	CommandManager::cmd_pass( Message& msg )
{ 
	Client& client	= *msg.get_client_ptr();

	if(msg[1].empty())
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	if ( client.is_password_validated()) 
	{
		run_reply(ERR_ALREADYREGISTERED, msg);
		return ;
	}
	if ( msg[1].compare(_server->get_password()) )
	{
		run_reply(ERR_PASSWDMISMATCH, msg);
		return ;
	}
	client.set_registration_flags(Client::PASS_SET);
	Server::log(string("Client fd ") + std::to_string(client.get_fd()) + " has entered the right password: " + msg[1]);
}
