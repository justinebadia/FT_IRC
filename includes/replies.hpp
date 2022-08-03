/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42quebec.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:47:19 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/03 15:17:34 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "Message.hpp"

namespace irc
{

void run_reply( int code, Message& msg );
void rpl_welcome( Message& msg );
/*NICK REPLIES*/
void err_nonicknamegiven( Message& msg);
void err_erroneusnickname( Message& msg);
void err_nicknameinuse( Message& msg);
void err_nickcollision( Message& msg);
/*USERS REPLIES*/
void err_nosuchserver( Message& msg);
void err_userdisabled( Message& msg );
void rpl_nousers( Message& msg );
void rpl_usersstart( Message& msg );
void rpl_endofusers( Message& msg );
/*USERS MSG REPLIES*/
void err_needmoreparams( Message& msg );
void err_alreadyregistered( Message& msg );



}

#endif