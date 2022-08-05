/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_temp.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 10:30:09 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include "utils.hpp"
#include "numeric_replies.hpp"

namespace irc
{

void	cmd_nick( Message& msg )
{ 
	Server& server			= Server::get_server();
	Client& client			= *msg.get_client_ptr();

	// std::cout << "in cmd_nick msg1 : " << msg[1] << std::endl;
	if ( !validate_entry(REGEX_NICKNAME, msg[1]) )
	{
		server.get_reply_ptr(ERR_ERRONEUSNICKNAME)(msg);
		return;
	}
	if ( server.get_client( msg[1] ) )
	{
		server.get_reply_ptr(ERR_NICKNAMEINUSE)(msg);
		return;
	}
	client.set_nickname(msg[1]);
	// std::cout << "Successfully set the nickname to " << msg[1];
}

void cmd_user( Message& msg )
{
	Server& server			= Server::get_server();
	Client& client			= *msg.get_client_ptr();

	if(msg[4].empty()) //checking if there is, at least, 5 parameters
	{
		server.get_reply_ptr(ERR_NEEDMOREPARAMS)(msg);
		return ;
	}
	if (!msg[1].empty())
		client.set_username(msg[1]);
	//if (msg[2].compare(0, msg[2].size(), client.get_hostname()) == 0) est ce qu'on check si le hostname est faux ?
	if (msg[4].find(":", 0) >= 0)
	{
		client.set_realname(msg.find_realname());
		// std::cout << client.get_realname() << std::endl;

	}	
	// On vérifie le hostname et le servername ?
	// Est ce qu'on doit afficher le timestamp ?? Ou on l'affiche quand le msg est buildé au complet
	
	return ;
}


// void	cmd_join( 

// }

// void	reply_nickname_exist( Message& msg)
// {
// 	msg.append_out("359"
// }

} // namespace irc end bracket

