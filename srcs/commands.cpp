/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 12:46:06 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"

namespace irc
{

void	cmd_nick( Message & msg )
{ 
	Server& server			= Server::get_server();
	Client& client			= *msg.get_client_ptr();

	if ( !server.get_client( client.get_nickname() ) )
	{
		// reply_nickname_exist( msg )
		// reply nickname already exist
		return;
	}
	client.set_nickname(msg[1]);

}

// void	cmd_join( 

// }

// void	reply_nickname_exist( Message& msg)
// {
// 	msg.append_out("359"
// }
