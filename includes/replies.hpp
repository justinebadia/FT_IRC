/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:47:19 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 18:20:29 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "Message.hpp"

namespace irc
{

void run_reply( int code, Message& msg );
void rpl_welcome( Message& msg );
void rpl_whoisuser( Message& msg );
void rpl_whoisserver(Message& msg );
void rpl_whoisoperator(Message& msg );
void rpl_endofwhois( Message& msg );
void rpl_whoischannels( Message& msg );
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