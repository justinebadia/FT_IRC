/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:47:19 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 17:10:08 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "Message.hpp"

namespace irc
{

void run_reply( int code, Message& msg );

void err_nonicknamegiven( Message & msg);

/*"<client> <nick> :Erroneus nickname"*/
void err_erroneusnickname( Message & msg);

/*"<client> <nick> :Nickname is already in use"
WARNING - est ce que client correspond à username?<>*/
void err_nicknameinuse( Message & msg);

/* "<nick> :Nickname collision KILL"*/
void err_nickcollision( Message & msg);

/*USER NUM_REPLIES*/
/*"<server name> :No such server"*/
void err_nosuchserver( Message & msg);

/* ":USERS has been disabled"*/
void err_userdisabled( Message & msg );
void rpl_nousers( Message & msg );

/* WARNING pas fini 
":UserID   Terminal  Host"*/
void rpl_usersstart( Message & msg );
void rpl_endofusers( Message & msg );

/*NUM REPLIES - Users message*/
void err_needmoreparams( Message & msg );
void err_alreadyregistered( Message & msg );

}

#endif