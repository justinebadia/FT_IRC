#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"


using namespace irc;

void CommandManager::cmd_kill( Message& msg )
{
	Client*	source_client = msg.get_client_ptr();

	if (source_client->is_operator() == false)
	{
		run_reply(ERR_NOPRIVILEGES, msg);
		return;
	}
	if (msg.get_param_count() < 2)
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (_database->is_client_listed(msg[1]) == false)
	{
		run_reply(ERR_NOSUCHNICK, msg);
		return;
	}
	if (_server->get_name() == msg[1])
	{
		run_reply(ERR_CANTKILLSERVER, msg);
		return;
	}

	Client*	target_client = _database->get_client(msg[1]);
	if (!target_client)
		return;
	target_client->set_to_be_killed(true);
;
	target_client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "KILL " + msg[1] + " :" + msg.get_substr_after(":") + CRLF);
	msg.append_out("KILL " + msg[1] + " :" + msg.get_substr_after(":") + CRLF);
	Message	quit_msg(target_client, "QUIT :Killed by operator " + source_client->get_nickname() + " for being naughty");
	cmd_quit(quit_msg);
	target_client->append_buff(BUFFOUT, "Error :Connection will close due to being killed by an operator" + CRLF);
}
