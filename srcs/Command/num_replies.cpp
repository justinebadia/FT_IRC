/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <tshimoda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 12:48:16 by jbadia            #+#    #+#             */
/*   Updated: 2022/08/12 20:16:53 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Database.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"

namespace irc 
{

void	CommandManager::run_reply( int code, Message& msg )
{
	t_reply_function_ptr reply_ptr;

	reply_ptr = get_reply_ptr(code);
	if (reply_ptr)
	{
		reply_ptr(msg);
		msg.append_out("\r\n");
	}
	else
		std::cout << GREEN << code << " reply function not found" << RESET << std::endl;
	return; 
}

void CommandManager::rpl_welcome( Message& msg )
{
	Client& client = *msg.get_client_ptr();

	msg.append_out(": 001 " + client.get_nickname() + " :Welcome to the Internet Relay Network " + client.get_hostname());
}

void CommandManager::rpl_whoisuser( Message& msg)
{
	Client& client = *msg.get_client_ptr();

	msg.append_out(": 311 : " + client.get_nickname() + " " + client.get_username() + " " + client.get_hostname() + " * " + client.get_realname());

}

void CommandManager::rpl_whoisserver(Message& msg )
{
	Client& client = *msg.get_client_ptr();
	
	msg.append_out(": 312 : " + client.get_nickname() + " " + _server->get_name());
}

void CommandManager::rpl_whoisoperator( Message& msg )
{
	Client& client = *msg.get_client_ptr();

	msg.append_out(": 313 : " + client.get_nickname() + " :is an IRC operator");
}

void CommandManager::rpl_endofwhois( Message& msg )
{
	Client& client = *msg.get_client_ptr();
	
	msg.append_out(": 318 : " + client.get_nickname() + " :End of WHOIS list");
}

void CommandManager::rpl_whoischannels( Message& )
{
	// Client& client = *msg.get_client_ptr();

	//if client est dans channel
		//if client est chanop
			//msg.append_out("319 " + client.get_nickname() + " :@channel"get_name()");
		//
		//else
			//msg.append_out("319 " + client.get_nickname() + " :");
}

/*NICK NUM_REPLIES*/
/* ":No nickname given"*/
void CommandManager::err_nonicknamegiven( Message& msg)
{
	msg.append_out(": 431 :No nickname given");
}

/*"<client> <nick> :Erroneus nickname"*/
void CommandManager::err_erroneusnickname( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ": 432 " + client.get_nickname() + " " + msg[1] + " :Erroneus nickname";
	msg.append_out(err_msg);
}

/*"<client> <nick> :Nickname is already in use"
WARNING - est ce que client correspond à username?<>*/
void CommandManager::err_nicknameinuse( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ": 433 " + client.get_nickname() + " " + msg[1] + " :Nickname is already in use";
	msg.append_out(err_msg);
}

/* "<nick> :Nickname collision KILL"*/
void CommandManager::err_nickcollision( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ": 436 " + client.get_nickname() + " :Nickname collision KILL";
	msg.append_out(err_msg);
}

/*USER NUM_REPLIES*/
/*"<server name> :No such server"*/
void CommandManager::err_nosuchserver( Message& msg)
{
	string err_msg = ": 402 " + msg[1] + " :No such server";
	msg.append_out(err_msg);
}

/* ":USERS has been disabled"*/
void CommandManager::err_userdisabled( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 446 :USERS has been disabled");
}

void CommandManager::rpl_nousers( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 395 :Nobody logged in");
}

/* WARNING pas fini 
":UserID   Terminal  Host"*/
void CommandManager::rpl_usersstart( Message&  )
{	
	string err_msg = "392 "; // WARNING todo à créer
}

void CommandManager::rpl_endofusers( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 394 End of users");
}

/*NUM REPLIES - Users message*/
void CommandManager::err_needmoreparams( Message& msg )
{
	string err_msg = ": 461 " + msg[0] + " :Not enough parameters";
	msg.append_out(err_msg);
}

void CommandManager::err_alreadyregistered( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 462 :You may not reregister";
	msg.append_out(err_msg);
}

void CommandManager::err_passwdmismatch( Message& msg )
{
	string err_msg = ": 464 :Not enough parameters";
	msg.append_out(err_msg);
}

void CommandManager::err_noorigin( Message& msg )
{
	msg.append_out(": 409 :No origin specified");
}

void CommandManager::rpl_topic( Message& msg )
{
	msg.append_out(": 332 " + msg.get_client_ptr()->get_nickname() + " " + msg[1] + " :"+ _database->get_channel(msg[1])->get_topic());
}

void CommandManager::rpl_notopic ( Message& msg )
{
	msg.append_out(": 331 " + msg[1] + " :No topic is set");
}

void CommandManager::err_nosuchchannel( Message& msg )
{
	msg.append_out(": 403 " + msg[1] + " :No such channel");
}

void CommandManager::err_badchanmask( Message& msg )
{
	msg.append_out(": 476 "); // WARNING RFC DIT PAS GRAND CHOSE
}

void CommandManager::err_chanoprivsneeded( Message& msg )
{
	msg.append_out(": 482 " + msg[1] + " :You're not channel operator");
}

void CommandManager::err_notonchannel( Message& msg )
{
	msg.append_out(": 442 " + msg[1] + " :You're not on that channel");
}








} // namespace irc end bracket
