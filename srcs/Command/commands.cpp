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
		return; // Not sure if its also ERR_NOSUCHNICK
		
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
		recipient_list.push_back(source_client);
		recipient_list.push_back(target_client);
		send_to_clients(recipient_list, source_client->get_prefix() + " INVITE " + msg[1] + " " + msg[2] + CRLF);
	}
}

/*[KILL]---------------------------------------------------------------------------------------------------------------[KILL]*/
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

/*[NICK]---------------------------------------------------------------------------------------------------------------[NICK]*/
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

/*[NOTICE]----------------------------------------------------------------------------------------------------------[NOTICE]*/
void	CommandManager::cmd_notice( Message& msg )
{ 
	t_client_ptr_list		recipient_list;
	Channel* 				channel;
	Client*					client;
	string					prefix;

	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		channel = _database->get_channel(msg[1]);
		if (channel)
		{
			client = msg.get_client_ptr();
			recipient_list = channel->get_clients_not_matching_permissions(BAN);
			recipient_list.remove(client);
			send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "NOTICE " + channel->get_name() + " " + msg.get_substr_after(":") + CRLF);
		}
	}
	else
	{
		client = _database->get_client(msg[1]);
		if (client)
			client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "NOTICE " + msg[1] + " " + msg.get_substr_after(":") + CRLF);
	}
	return;
	
}

/*[OPER]---------------------------------------------------------------------------------------------------------------[OPER]*/
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

/*[PART]---------------------------------------------------------------------------------------------------------------[PART]*/


/*[PASS]---------------------------------------------------------------------------------------------------------------[PASS]*/
void	CommandManager::cmd_pass( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();

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

/*[PING]---------------------------------------------------------------------------------------------------------------[PING]*/
void CommandManager::cmd_ping( Message& msg )
{
	if (!msg[1].empty() && msg[1] != _server->get_server_ip() && !_database->get_client(msg[1]))
		run_reply(ERR_NOSUCHSERVER, msg);
	else
		msg.append_out("PONG :" + _server->get_server_ip());
		
	return;
}


/*[QUIT]---------------------------------------------------------------------------------------------------------------[QUIT]*/
void CommandManager::cmd_quit( Message& msg )
{
	Client*				client	= msg.get_client_ptr();
	string				prefix = client->get_prefix();
	t_channel_ptr_list	chan_list = _database->get_channel_list_of_client(client);

	if (!client)
		return;
	client->set_to_be_killed(true);
	msg.set_client_ptr(NULL);
	if (!msg[1].empty())
		send_to_channels(chan_list, prefix + "QUIT :Quit: " + msg.get_substr_after(":") + CRLF);	
	if (msg[1].empty())
		send_to_channels(chan_list, prefix + "QUIT :Quit: No reason given" + CRLF);
	msg.append_out("Error :connection will be closed..." + CRLF);
}

/*[TOPIC]-------------------------------------------------------------------------------------------------------------[TOPIC]*/
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
		if (topic.empty() == true)
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


/*[USER]---------------------------------------------------------------------------------------------------------------[USER]*/
void	CommandManager::cmd_user( Message& msg )
{
	Client& client			= *msg.get_client_ptr();

	if(msg[4].empty() || msg.get_substr_after(":").empty()) //checking if there is, at least, 5 parameters
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	if(!client.get_username().empty())
		return(run_reply(ERR_ALREADYREGISTERED, msg));
	if (!msg[1].empty())
		client.set_username(msg[1]);
	if ( !msg.get_substr_after(":").empty() )
		client.set_realname(msg.get_substr_after(":"));
	if (msg[3].length() == 1 && msg[3][0] == '*')
		client.set_hostname(_server->get_name());
	else
		client.set_hostname(msg[3]);
	client.set_registration_flags(Client::USER_SET);
	Server::log("Successfully set the username to " + msg.get_substr_after(":") + RESET);
	return ;
}

/*[WHOIS]-------------------------------------------------------------------------------------------------------------[WHOIS]*/
void CommandManager::cmd_whois( Message & msg )
{
	size_t pos = 0;
	size_t first = 0;
	string nick_to_test;
	string nick_list;
	t_channel_ptr_list channels;

	if (msg.get_param_count() > 2)
	{
		if ( msg[1] != _server->get_server_ip())
		{
			run_reply(ERR_NOSUCHSERVER, msg);
			return ;
		}
		nick_list = msg[2];
	}
	else
		nick_list = msg[1];
	while (pos < nick_list.length())
	{
		pos = nick_list.find(",", first);
		nick_to_test = nick_list.substr(first, pos);
		Client* client = _database->get_client(nick_to_test);			
		if (client)
		{
			run_reply(RPL_WHOISUSER, msg);
			run_reply(RPL_WHOISSERVER, msg);
			if (client->is_operator())
				run_reply(RPL_WHOISOPERATOR, msg);
			channels = _database->get_channel_list_of_client(client);
			if (channels.size() > 0)
				run_reply(RPL_WHOISCHANNELS, msg);
			run_reply(RPL_ENDOFWHOIS, msg);

		}
		first = pos + 1;
	}
	return;
}
