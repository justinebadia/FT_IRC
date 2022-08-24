#include <map>

#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "Channel.hpp"


using namespace irc;
using std::cout;
using std::cerr;
using std::endl;

void CommandManager::cmd_privmsg( Message& msg )
{
	t_client_ptr_list		recipient_list;
	Channel* 				channel;
	Client*					client;
	string					prefix;

	if (msg.get_param_count() < 1 || msg[1][0] == ':')
		return run_reply(ERR_NORECIPIENT, msg);
	
	client = _database->get_client(msg[1]);
	channel = _database->get_channel(msg[1]);
	if (!client && !channel)
		return run_reply(ERR_NOSUCHNICK, msg);

	if (msg.get_substr_after(":").empty())
		return run_reply(ERR_NOTEXTTOSEND, msg);

	if (client)
		client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "PRIVMSG " + msg[1] + " :" + msg.get_substr_after(":") + CRLF);
	else
	{
		client = msg.get_client_ptr();
		if (channel->is_banned(client))
			return run_reply(ERR_CANNOTSENDTOCHAN, msg);
		recipient_list = channel->get_clients_not_banned();
		recipient_list.remove(client);
		send_to_clients(recipient_list, client->get_prefix() + "PRIVMSG " + channel->get_name() + " :" + msg.get_substr_after(":") + CRLF);
	}
	return;
}
