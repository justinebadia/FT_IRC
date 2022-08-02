/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   num_replies.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 12:48:16 by jbadia            #+#    #+#             */
/*   Updated: 2022/08/02 16:32:16 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Message/Message.hpp"
#include "Server.hpp"
#include "numeric_replies.hpp"
#include "Channel/Channel.hpp"
#include "typedef.hpp"

namespace irc 
{

void run_reply( int code, Message& msg )
{
	Server&	server = Server::get_server();
	t_reply_function_ptr reply_ptr;

	reply_ptr = server.get_reply_ptr(code);
	if (reply_ptr)
		reply_ptr(msg);
	else
		std::cout << code << " reply function not found" << std::endl;
	return; 
}

/*NICK NUM_REPLIES*/
/* ":No nickname given"*/
void err_nonicknamegiven( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 431 :No nickname given");
}

/*"<client> <nick> :Erroneus nickname"*/
void err_erroneusnickname( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() +" 432 " + client.get_username() + " " + client.get_nickname() + " :Erroneus nickname";
	msg.append_out(err_msg);
}

/*"<client> <nick> :Nickname is already in use"
WARNING - est ce que client correspond à username?<>*/
void err_nicknameinuse( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 433 " + client.get_username() + " " + client.get_nickname() + " :Nickname is already in use";
	msg.append_out(err_msg);
}

/* "<nick> :Nickname collision KILL"*/
void err_nickcollision( Message& msg)
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 436 " + client.get_nickname() + " :Nickname collision KILL";
	msg.append_out(err_msg);
}

/*USER NUM_REPLIES*/
/*"<server name> :No such server"*/
void err_nosuchserver( Message& msg)
{
	Server &server = Server::get_server();
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 401 " + server.get_server_name() + " :No such server"; //à créer
	msg.append_out(err_msg);
}

/* ":USERS has been disabled"*/
void err_userdisabled( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 446 :USERS has been disabled");
}

void rpl_nousers( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 395 :Nobody logged in");
}

/* WARNING pas fini 
":UserID   Terminal  Host"*/
void rpl_usersstart( Message& msg )
{
	Server &server = Server::get_server();
	
	string err_msg = "392 "; //à créer
}

void rpl_endofusers( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	msg.append_out(":" + client.get_hostname() + " 394 End of users");
}

/*NUM REPLIES - Users message*/
void err_needmoreparams( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 461 " + msg[0] + " :Not enough parameters";
}

void err_alreadyregistered( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ":" + client.get_hostname() + " 462 :You may not reregistered";
}
//WARNING - est ce qu'on fait 424 ERR_FILEERROR ??


// reply_function = server.get_reply_map().find(RPL_LISTSTART);
// reply_function(msg);

} // namespace irc end bracket

