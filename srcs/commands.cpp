/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 16:58:57 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "commands.hpp"
#include "utils.hpp"
#include "numeric_replies.hpp"
#include "replies.hpp"

namespace irc
{

void	cmd_nick( Message & msg )
{ 
	Server& server			= Server::get_server();
	Client& client			= *msg.get_client_ptr();

	if ( !validate_entry(REGEX_NICKNAME, msg[1]) )
	{
		run_reply(ERR_ERRONEUSNICKNAME, msg);
		return;
	}
	if ( server.get_client( msg[1] ) )
	{
		run_reply(ERR_NICKNAMEINUSE, msg);
		return;
	}
	client.set_nickname(msg[1]);
	std::cout << "Successfully set the nickname to " << msg[1];
}

}

// void	cmd_join( 

// }

// void	reply_nickname_exist( Message& msg)
// {
// 	msg.append_out("359"
// }
