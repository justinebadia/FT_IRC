/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/15 11:36:11 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (msg.get_param_count() < 2)				// WARNING à verfier
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (_database->is_client_listed(msg[1]) == false)		// if target is not in the client list
	{
		run_reply(ERR_NOSUCHNICK, msg);
		return;
	}

	Channel* channel = _database->get_channel(msg[2]);	// if the channel doesnt exist
	if (!channel)
		return; // Not sure if its also ERR_NOSUCHNICK
		
	if (channel->is_member(source_client) == false)		// if the source is not a member of the channel
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}

	Client* target_client = _database->get_client(msg[1]);
	if (channel->is_member(target_client) && channel->get_permission(target_client) != BAN)	// if target user is already a member of the channel && is also not ban!!!
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
		_database->create_invite_coupon(target_client, channel);
	run_reply(RPL_INVITING, msg);
	
	t_client_ptr_list	recipient_list;
	recipient_list.push_back(source_client);
	recipient_list.push_back(target_client);
	send_to_clients(recipient_list, source_client->get_prefix() + " INVITE " + msg[1] + " " + msg[2] + CRLF);
	
	// RPL_AWAY ??? not sure
}

/*[JOIN]---------------------------------------------------------------------------------------------------------------[JOIN]*/
void CommandManager::cmd_join( Message& msg )			
{
	t_client_ptr_list	client_list;
	Channel* 			channel;
	string topic;
	Client* client_ptr = msg.get_client_ptr();	
	if (msg[1].empty())
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (_database->get_channel_count() >= MAX_CHANNELS)
	{
		run_reply(ERR_TOOMANYCHANNELS, msg);
		return;
	}
	
	channel = _database->get_channel(msg[1]);
	if (!channel) // if channel == null : create a new one
	{
		_database->add_channel_list(Channel(string(msg[1]), msg.get_client_ptr()));
		channel = _database->get_channel(msg[1]);
	}
	else if (channel->get_memberlist().size() >= MAX_CLIENT_PER_CHAN)
	{
		run_reply(ERR_CHANNELISFULL, msg);
		return;
	}
	else if (channel->is_banned(client_ptr))
	{
		run_reply(ERR_BANNEDFROMCHAN, msg);
		return ;
	}
	else if (channel->is_member(client_ptr))
	{
		return ;
	}
	else if (channel->get_is_invite_only())
	{
		//if (INVITED == true)
		//{	
		//	INVITED = false;
		//	channel->add_member(client_ptr, REGULAR);
		//  
		//}
		//else
		//
			run_reply(ERR_INVITEONLYCHAN, msg);
			return ;
	}
	//_database->add_client_to_channel(msg.get_client_ptr(), channel);
	
	channel->add_member(client_ptr, REGULAR);

	topic = channel->get_topic();
	if (topic.size() == 0)				// WARNING A VERIFIER si NULL ou string vide ""
		run_reply(RPL_NOTOPIC, msg);
	else
		run_reply(RPL_TOPIC, msg);
			
	// WARNING ERR_BADCHANMASK ???
	// WARNING ERR_NOSUCHCHANNEL ???
	client_list = channel->get_clients_not_matching_permissions(BAN);
	send_to_clients(client_list, client_ptr->get_prefix() + " JOIN " + msg[1] + CRLF);

	return;
}

/*[KICK]---------------------------------------------------------------------------------------------------------------[KICK]*/
void CommandManager::cmd_kick( Message& msg )
{
	if (msg.get_param_count() < 2)				// WARNING à verfier
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	
	Client*	source_client = msg.get_client_ptr();
	t_client_ptr_list recipient_list;
	
	Channel* channel = _database->get_channel(msg[1]);
	if (!channel)
	{
		run_reply(ERR_NOSUCHCHANNEL, msg);
		return ;
	}
	if (msg[1][0] != '&' && msg[1][0] != '#')
	{
		run_reply(ERR_BADCHANMASK, msg); 
		return;
	}
	if (channel->is_member(source_client) == false)
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}
	if (!channel->is_owner(source_client) && !channel->is_chanop(source_client))
	{	
		run_reply(ERR_CHANOPRIVSNEEDED, msg);
		return;
	}
	recipient_list = channel->get_clients_not_matching_permissions(BAN);
	send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "KICK " + msg[1] + " " + msg[2] + CRLF);
	
	Client* target_client = _database->get_client(msg[2]);
	if (!target_client)	// WARNING what are we supposed to do? its not ERR_NOTONCHANNEL
		return;
	channel->remove_member(target_client);

	// WARNING TEST TO DO try chanop kick another chanop and what happens when you kick a banned member? is removed from memberlist?
}


/*[MODE]-------------------------------------------in-seperate-file-MODE.cpp-------------------------------------------[MODE]*/


/*[NICK]---------------------------------------------------------------------------------------------------------------[NICK]*/
void	CommandManager::cmd_nick( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();
	
	if ( !validate_entry(REGEX_NICKNAME, msg[1]) )
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
	Server::log(string() + GREEN + "Successfully set the nickname to " + msg[1] + RESET);
	client.append_buff(BUFFOUT, ":" + client.get_nickname() + " NICK " + msg[1] + CRLF);
	client.set_nickname(msg[1]);
}

/*[OPER]---------------------------------------------------------------------------------------------------------------[OPER]*/
void	CommandManager::cmd_oper( Message& msg )
{
	if(msg[1].empty())
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	_server->attempt_client_as_operator(*msg.get_client_ptr(), msg[1], msg[2]);
}

/*[PART]---------------------------------------------------------------------------------------------------------------[PART]*/
void	CommandManager::cmd_part( Message& msg )
{
	Client* source_client = msg.get_client_ptr();	
	if (msg.get_param_count() < 1)				// WARNING à verfier
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
}


/*[PASS]---------------------------------------------------------------------------------------------------------------[PASS]*/
void	CommandManager::cmd_pass( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();

	if(msg[1].empty())
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	if ( client.is_registered() )
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
	// if (msg[1].empty())
	// 	run_reply(ERR_NOORIGIN, msg);
	if (!msg[1].empty() && msg[1] != _server->get_name() && !_database->get_client(msg[1]))
		run_reply(ERR_NOSUCHSERVER, msg);
	else
		msg.append_out("PONG :" + _server->get_name());
		
	return;
}

/*[PRIVMSG]---------------------------------------------------------------------------------------------------------[PRIVMSG]*/
void CommandManager::cmd_privmsg( Message& msg ) // WARNING done minimally for channel testing
{
	t_client_ptr_list		recipient_list;
	Channel* 				channel;
	Client*					client;
	string					prefix;

	if (msg[1].empty())
	{
		// run_reply(ERR_NORECIPIENT, msg);
		return;
	}
	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		channel = _database->get_channel(msg[1]);
		if (channel)
		{
			client = msg.get_client_ptr();
			recipient_list = channel->get_clients_not_matching_permissions(BAN);
			recipient_list.remove(client);
			send_to_clients(recipient_list, msg.get_client_ptr()->get_prefix() + "PRIVMSG " + channel->get_name() + " " + msg.get_substr_after(":") + CRLF);
		}
	}
	else
	{
		client = _database->get_client(msg[1]);
		if (client)
			client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "PRIVMSG " + msg[1] + " " + msg.get_substr_after(":") + CRLF);
	}
	
	return;
}

/*[QUIT]---------------------------------------------------------------------------------------------------------------[QUIT]*/
void CommandManager::cmd_quit( Message& msg )// WARNING not sending the right stuff at all
{
	Client*				client	= msg.get_client_ptr();
	string				prefix = client->get_prefix();
	t_channel_ptr_list	chan_list = _database->get_channel_list_of_client(client);

	if (!client)
		return;
	_server->disconnect_client(client->get_fd()); // WARNING to be changed for a queue of client to disconnect, I think?
	msg.set_client_ptr(NULL);
	cout << "Amount of channels client is in : " << chan_list.size() << endl;
	if (!msg[1].empty())  // WARNING todo
		send_to_channels(chan_list, prefix + "QUIT :" + msg.get_substr_after(":") + CRLF);	
	if (msg[1].empty())  // WARNING todo
		send_to_channels(chan_list, prefix + "QUIT" + CRLF);
}

/*[TOPIC]-------------------------------------------------------------------------------------------------------------[TOPIC]*/

void	CommandManager::cmd_topic( Message& msg )
{
	t_client_ptr_list	client_list;
	Client*				source_client = msg.get_client_ptr();
	Channel*			channel = _database->get_channel(msg[1]);
	string				topic;

	if (msg.get_param_count() < 1)				// WARNING à verfier
		run_reply(ERR_NEEDMOREPARAMS, msg);

	if (!channel || channel->is_member(source_client) == false)
	{	
		run_reply(ERR_NOTONCHANNEL, msg);
		return;
	}

	topic = channel->get_topic();

	if (msg.get_param_count() == 1)				// SHOW TOPIC
	{
		if (topic.size() == 0)				// WARNING A VERIFIER si NULL ou string vide ""
			run_reply(RPL_NOTOPIC, msg);
		else
			run_reply(RPL_TOPIC, msg);
			
		client_list.push_back (source_client);
		send_to_clients(client_list, msg.get_client_ptr()->get_prefix() + "TOPIC " + msg[1] + " " + topic + CRLF);
	}
	else if (msg.get_param_count() == 2)
	{
		if (channel->get_is_topic_by_chanop_only() == true)
		{
			if (!channel->is_owner(source_client) && !channel->is_chanop(source_client))
			{	
				run_reply(ERR_CHANOPRIVSNEEDED, msg);
				return;
			}
		}
		channel->set_topic(msg[2]);
		// SEND A MESAGE ???
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
	if (!msg[1].empty())
		client.set_username(msg[1]);
	if ( !msg.get_substr_after(":").empty() )
		client.set_realname(msg.get_substr_after(":"));
	if (msg[3].length() == 1 && msg[3][0] == '*')
		client.set_hostname(_server->get_name());
	else
		client.set_hostname(msg[3]);
	client.set_registration_flags(Client::USER_SET);
	Server::log(string() + GREEN + "Successfully set the username to " + msg.get_substr_after(":") + RESET);
	return ;
}

/*[WHOIS]-------------------------------------------------------------------------------------------------------------[WHOIS]*/
void CommandManager::cmd_whois( Message & msg )
{
	if (!msg[2].empty())
	{
		if (msg[2] != _server->get_name())
			get_reply_ptr(ERR_NOSUCHSERVER)(msg);
		return;
	}
	run_reply(RPL_WHOISUSER, msg);
	run_reply(RPL_WHOISSERVER, msg);
	run_reply(RPL_WHOISOPERATOR, msg);
	//get_reply_ptr(RPL_WHOISCHANNELS)(msg); WARNING A FAIRE
	run_reply(RPL_ENDOFWHOIS, msg); //signifie que c'est la fin de la querry WHOIS

	return;
}
