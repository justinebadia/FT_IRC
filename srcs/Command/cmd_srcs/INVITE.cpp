#include "CommandManager.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "Channel.hpp"


using namespace irc;

/*[INVITE]-----------------------------------------------------------------------------------------------------------[INVITE]*/
void CommandManager::cmd_invite( Message& msg )
{
	// NOTE INVITE ONLY WORKS FOR INVITE_ONLY CHANNEL (+i)
	
	Client* source_client = msg.get_client_ptr();	
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
	Channel* channel = _database->get_channel(msg[2]);	// if the channel doesnt exist
	if (!channel)
	{
		run_reply(ERR_NOSUCHNICK, msg);	// RFC N'A PAS MIS POUR INVITE : NO SUCH CHANNEL
		return;
	}
	if (channel->is_member(source_client) == false)
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}
	Client* target_client = _database->get_client(msg[1]);
	if (channel->is_member(target_client))
	{
		run_reply(ERR_USERONCHANNEL, msg);
		return;
	}
	if (!channel->is_owner(source_client) && !channel->is_chanop(source_client))
	{	
		run_reply(ERR_CHANOPRIVSNEEDED, msg);
		return;
	}
	if (channel->get_is_invite_only() == true)
	{
		_database->create_invite_coupon(target_client, channel);
		run_reply(RPL_INVITING, msg);
	
		t_client_ptr_list	recipient_list;
	//	recipient_list.push_back(source_client);
		recipient_list.push_back(target_client);
		send_to_clients(recipient_list, source_client->get_prefix() + " INVITE " + msg[1] + " " + msg[2] + CRLF);
	}
}
