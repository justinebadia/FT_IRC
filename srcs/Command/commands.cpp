/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/12 15:42:33 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"

using namespace irc;
using std::cout;
using std::cerr;
using std::endl;


void CommandManager::cmd_join( Message& msg )
{
	t_client_ptr_list	chan_memberlist;
	Channel* 			channel;
	string topic;
	Client& client = *msg.get_client_ptr();
	
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
	chan_memberlist = _database->get_clients_in_channel(channel);
	if (!channel)
	{
		_database->add_channel_list(Channel(string(msg[1]), msg.get_client_ptr()));
		channel = _database->get_channel(msg[1]);
	}
	else if (chan_memberlist.size() >= MAX_CLIENT_PER_CHAN)
	{
		run_reply(ERR_CHANNELISFULL, msg);
		return;
	}
	else if (channel->is_banned(msg.get_client_ptr()))
	{
		run_reply(ERR_BANNEDFROMCHAN, msg);
		return ;
	}
	else if (channel->get_is_invite_only())
	{
		run_reply(ERR_INVITEONLYCHAN, msg);
		return ;
	}
	_database->add_client_to_channel(msg.get_client_ptr(), channel);
	topic = channel->get_topic();
	run_reply(RPL_TOPIC, msg);
	client.append_buff(BUFFOUT, client.get_prefix() + " JOIN " + msg[1] + CRLF);
	// WARNING ERR_BADCHANMASK
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL??
	return;
}

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

void CommandManager::cmd_privmsg( Message& msg ) // WARNING done minimally for channel testing
{
	t_client_ptr_list	chan_memberlist;
	Channel* 			channel;
	Client*				client;
	string				prefix;

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
			chan_memberlist = _database->get_clients_in_channel(channel);
			chan_memberlist.remove(msg.get_client_ptr());
			send_to_clients(chan_memberlist, msg.get_client_ptr()->get_prefix() + "PRIVMSG " + channel->get_name() + " :" + msg.get_colon() + CRLF);
		}
	}
	else
	{
		client = _database->get_client(msg[1]);
		if (client)
			client->append_buff(BUFFOUT, msg.get_client_ptr()->get_prefix() + "PRIVMSG " + msg[1] + " :" + msg.get_colon() + CRLF);
	}
	
	return;
}

void	CommandManager::cmd_user( Message& msg )
{
	Client& client			= *msg.get_client_ptr();

	if(msg[4].empty() || msg.get_colon().empty()) //checking if there is, at least, 5 parameters
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	if (!msg[1].empty())
		client.set_username(msg[1]);
	if ( !msg.get_colon().empty() )
		client.set_realname(msg.get_colon());
	if (msg[3].length() == 1 && msg[3][0] == '*')
		client.set_hostname(_server->get_name());
	else
		client.set_hostname(msg[3]);
	client.set_registration_flags(Client::USER_SET);
	Server::log(string() + GREEN + "Successfully set the username to " + msg.get_colon() + RESET);
	return ;
}

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


void CommandManager::cmd_quit( Message& msg )// WARNING not sending the right stuff at all
{
	Client*				client	= msg.get_client_ptr();
	string				prefix = client->get_prefix();
	t_channel_ptr_list	chan_list = _database->get_channel_list_of_client(client);

	if (!client)
		return;
	_database->remove_client_list(client); // WARNING to be changed for a queue of client to disconnect, I think?
	cout << "Amount of channels client is in : " << chan_list.size() << endl;
	if (!msg[1].empty())  // WARNING todo
		send_to_channels(chan_list, prefix + "QUIT :" + msg.get_colon() + CRLF);	
	if (msg[1].empty())  // WARNING todo
		send_to_channels(chan_list, prefix + "QUIT" + CRLF);
}


void CommandManager::cmd_kick( Message& msg )
{
//	if (msg.get_param_count() < 2)
//		run_reply(ERR_NEEDMOREPARAMS, msg);
	
	 	Client*	source_client = msg.get_client_ptr();
		
		Channel* channel = _database->get_channel(msg[1]);
		if (!channel)
			run_reply(ERR_NOSUCHCHANNEL, msg);
		
		if (!msg[1][0] == '&' && msg[1][0] != '#')
			run_reply(ERR_BADCHANMASK, msg); 
		
		if (channel->is_chanop(source_client) == false)
			run_reply(ERR_CHANOPRIVSNEEDED, msg);
		
		if (!msg[2].empty())
		{
			Client* target_client = _database->get_client(msg[2]);
			if (!target_client)
				run_reply(ERR_NOTONCHANNEL, msg);
			if (channel->is_member(target_client) == false)
				run_reply(ERR_NOTONCHANNEL, msg);
			channel->remove_member(target_client);
		}			
}

