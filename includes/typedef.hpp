/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 12:54:52 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 17:30:40 by fousse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <utility>
#include <map>
#include <list>
#include <netinet/in.h>
#include <poll.h>

using std::string;
using std::map;
using std::list;
using std::pair;

namespace irc {

class Client;
class Message;

typedef	void(*t_cmd_function_ptr)(Message&);
typedef	void(*t_reply_function_ptr)(Message&);

typedef list<Client>						t_client_list;
typedef list<Client*>						t_client_ptr_list;
typedef list<Channel>						t_channel_list;
typedef list<Channel*>						t_channel_ptr_list;
typedef map<string, t_client_ptr_list>		t_channel_clients_map;

typedef	pair<string, t_cmd_function_ptr>	t_cmd_pair;
typedef	pair<int, t_reply_function_ptr>		t_reply_pair;

typedef map<string, t_cmd_function_ptr>		t_command_map;
typedef map<int, t_reply_function_ptr>		t_reply_map;

// typedef map<int, 

// typedef map<string, string>				t_message_info; // WARNING: might not use it

typedef struct	pollfd					t_pollfd;
typedef struct	sockaddr_in6			t_addr6;
//typedef struct	sockaddr_in				t_addr;



typedef struct s_socket
{
       t_pollfd			pollfd; // int fd, short events & revents
       t_addr6			addr6; 
}      t_socket;


} // namespace irc end bracket

/* NOTE SOCKADDR_IN6 */
 
// struct sockaddr_in6 
//     {
//        sa_family_t     sin6_family;   /* AF_INET6 */
//                in_port_t       sin6_port;     /* port number */
//                uint32_t        sin6_flowinfo; /* IPv6 flow information */
//                struct in6_addr sin6_addr;     /* IPv6 address */
//                uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
//     };


//        struct in6_addr 
//        {
//                unsigned char   s6_addr[16];   /* IPv6 address */
//        };

#endif

