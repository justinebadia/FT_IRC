#include "CommandManager.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"

using namespace irc;

void	CommandManager::cmd_topic( Message& msg )
{
	t_client_ptr_list	recipient_list;
	Client*				source_client = msg.get_client_ptr();
	Channel*			channel = _database->get_channel(msg[1]);
	string				topic;

	if (msg.get_param_count() < 1)
		run_reply(ERR_NEEDMOREPARAMS, msg);

	if (!channel || channel->is_member(source_client) == false)
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}
	topic = channel->get_topic();
	if (msg.get_param_count() == 1)				// SHOW TOPIC
	{
		if (topic.empty() == true)				// WARNING A VERIFIER si NULL ou string vide ""
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
	}
	else if (msg.get_param_count() >= 2)
	{
		if (channel->get_is_topic_by_chanop_only() == true)
		{
			if (!channel->is_owner(source_client) && !channel->is_chanop(source_client))
			{	
				run_reply(ERR_CHANOPRIVSNEEDED, msg);
				return;
			}
		}
		channel->set_topic(msg.get_substr_after(":"));
		recipient_list = channel->get_clients_any_permissions();
		send_to_clients(recipient_list, source_client->get_prefix() + "TOPIC " + msg[1] + " :" + msg.get_substr_after(":") + CRLF);
	}
}
