/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_temp.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 14:20:37 by jbadia           ###   ########.fr       */
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

	std::cout << "in cmd_nick msg1 : " << msg[1] << std::endl;
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
	std::cout << "Successfully set the nickname to " << msg[1];
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

	
	/*Command: USER -  Parameters: <username> <hostname> <servername> <realname>*/
	/*split la ligne sur les espaces pour vérifier si param ‹ 5 sinon code 461NEEDMOREPARAM
	On va find le realname grace au : et on vérifie s'il n'a pas déjà été donné 
	s'il a déjà été donné ?? code 462 ?? 
	On vérifie le hostname et le servername ?
	On checke le username et on le set ? 
	
	Est ce qu'on doit afficher le timestamp ?? Ou on l'affiche quand le msg est buildé au complet*/

	return ;
}


// void	cmd_join( 

// }

// void	reply_nickname_exist( Message& msg)
// {
// 	msg.append_out("359"
// }

} // namespace irc end bracket

