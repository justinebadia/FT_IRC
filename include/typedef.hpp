/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 12:54:52 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 14:38:17 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <list>
#include <netinet/in.h>
#include <poll.h>

namespace irc {

class Client;
class Server;

typedef std::list<Client>			t_client_list;
typedef struct	pollfd				t_pollfd;
typedef struct	sockaddr_in6		t_addr6;
typedef int							size_fd; // change this if we wanna use another data type

}
