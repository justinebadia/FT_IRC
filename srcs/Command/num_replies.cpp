/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42quebec.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 12:48:16 by jbadia            #+#    #+#             */
/*   Updated: 2022/08/08 13:49:05 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "numeric_replies.hpp"
#include "../Channel/Channel.hpp"
#include "typedef.hpp"
#include "CommandManager.hpp"
#include "../includes/color.hpp"

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

	msg.append_out("001 :Welcome to the Internet Relay Network " + client.get_hostname());
}

void CommandManager::rpl_whoisuser( Message& msg)
{
	Client& client = *msg.get_client_ptr();

	msg.append_out("311 : " + client.get_nickname() + " " + client.get_username() + " " + client.get_hostname() + " * " + client.get_realname());

}

void CommandManager::rpl_whoisserver(Message& msg )
{
	Client& client = *msg.get_client_ptr();
	
	msg.append_out("312 : " + client.get_nickname() + " " + _server->get_name());
}

void CommandManager::rpl_whoisoperator( Message& msg )
{
	Client& client = *msg.get_client_ptr();

	msg.append_out("313 : " + client.get_nickname() + " :is an IRC operator");
}

void CommandManager::rpl_endofwhois( Message& msg )
{
	Client& client = *msg.get_client_ptr();
	
	msg.append_out("318 : " + client.get_nickname() + " :End of WHOIS list");
}

void CommandManager::rpl_whoischannels( Message& msg )
{
	Client& client = *msg.get_client_ptr();

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
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 431 :No nickname given");
}

/*"<client> <nick> :Erroneus nickname"*/
void CommandManager::err_erroneusnickname( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 432 " + client.get_username() + " " + client.get_nickname() + " :Erroneus nickname";
	msg.append_out(err_msg);
}

/*"<client> <nick> :Nickname is already in use"
WARNING - est ce que client correspond à username?<>*/
void CommandManager::err_nicknameinuse( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 433 " + client.get_username() + client.get_nickname() + " :Nickname is already in use";
	msg.append_out(err_msg);
}

/* "<nick> :Nickname collision KILL"*/
void CommandManager::err_nickcollision( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 436 " + client.get_nickname() + " :Nickname collision KILL";
	msg.append_out(err_msg);
}

/*USER NUM_REPLIES*/
/*"<server name> :No such server"*/
void CommandManager::err_nosuchserver( Message& msg)
{
	Server&	server = Server::get_server();
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 402 " + server.get_name() + " :No such server";
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
void CommandManager::rpl_usersstart( Message& msg )
{	
	string err_msg = "392 "; //à créer
}

void CommandManager::rpl_endofusers( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 394 End of users");
}

/*NUM REPLIES - Users message*/
void CommandManager::err_needmoreparams( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 461 " + msg[0] + " :Not enough parameters";
	msg.append_out(err_msg);
}

void CommandManager::err_alreadyregistered( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 462 :You may not reregistered";
	msg.append_out(err_msg);
}

void CommandManager::err_noorigin( Message& msg )
{
	msg.append_out("409 :No origin specified");
}

} // namespace irc end bracket

