/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/31 14:29:22 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <map>
#include <list>

#include "irc_define.hpp"
#include "typedef.hpp"
#include "Client.hpp"

using std::string;
using std::map;

class Client;
class Channel;
class Message;

namespace irc {

class Socket 
{
private:
	static socklist		_socket_list;
	static size_t		_socket_count;

	t_pollfd	_pollfd; // pointer to the socket's fd, aka not an array
	t_sockaddr6	_address;
	string		_buff[2];
	bool		_opened;
	
	/* Prohibited constructor */
	Socket( void ) {  };
	Socket( const Socket & other ) {  };
	Socket& operator=( const Socket& other ) { return *this; };

public:
	/* Constructors */
	Socket( size_fd fd );
	
	~Socket( void );
	

	/* Getters */
	static const sockmap	get_socket_map ( void );
	static t_pollfd*		get_pollfd_array ( void );
	static size_t			get_socket_count ( void );
	t_pollfd*				get_pollfd_ptr ( void ) const;
	t_pollfd&				get_pollfd ( void ) const;
	const t_sockaddr6&		get_sock_addr ( void ) const;
	const size_fd&			get_fd ( void ) const;
	short					get_revents ( void ) const;
	short					get_events ( void ) const;
	string&					get_buff ( u_int buff_i );

	/* Setters */
	void	set_events ( const short & fde );
	void	set_sock_addr ( const t_sockaddr6 & addr );
	void	set_pollfd ( t_pollfd * pfd );
	void	set_fd ( const size_fd & fd );

	/* Utils */
	bool	is_event ( int event ) const ;
	bool	is_opened ( void ) const;


};

} // namespace bracket

#endif