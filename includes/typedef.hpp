/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 12:54:52 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 12:57:10 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <list>
#include <netinet/in.h>
#include <poll.h>

using std::string;
using std::map;
using std::list;

namespace irc {

class Client;
class Message;

typedef list<Client>					t_client_list;

typedef									void(*cmd_function_ptr)(Message&);
typedef									void(*cmd_function_ptr)(Message&);
typedef map<string, cmd_function_ptr>	t_command_map;
typedef map<int, 

// typedef map<string, string>				t_message_info; // WARNING: might not use it

typedef struct	pollfd					t_pollfd;
typedef struct	sockaddr_in6			t_addr6;

}
