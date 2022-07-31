/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/31 14:43:57 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_define.hpp"
#include "typedef.hpp"
#include "Client.hpp"
#include "Socket.hpp"

using namespace irc;

/* static initialization */
t_client_list	_client_list;
size_t			_socket_count = 0;



Socket::~Socket( void ) {  }

/* static getters */
const sockmap&	get_socket_map ( void ) { return _socket_map; }
t_pollfd*		get_pollfd_array ( void ) { return _poll_array; }
size_t			get_socket_count ( void ) { return _socket_count; }

/* getters */
t_pollfd*			Socket::get_pollfd_ptr ( void ) const{ return _pollfd; }
t_pollfd&			Socket::get_pollfd ( void ) const { return *_pollfd; }
const t_sockaddr6&	Socket::get_sock_addr ( void ) const { return _address; }
const size_fd&		Socket::get_fd ( void ) const { return _pollfd->fd; }
short				Socket::get_events ( void ) const { return (get_pollfd().events); }
short				Socket::get_revents ( void ) const { return (get_pollfd().revents); }
string&				Socket::get_buff ( u_int buff_i ) 
{ 
	if (buff_i == BUFFIN)
		return _buffin;
	if (buff_i == BUFFOUT)
		return _buffout;
	return _buffin;
}

/* setters */
void	Socket::set_pollfd ( t_pollfd * pfd ) { _pollfd = pfd; }
void	Socket::set_fd ( const size_fd & fd ) { get_pollfd().fd = fd; }
void	Socket::set_events ( const short & fde )	{ get_pollfd().events = fde; }

/* utils */
bool	Socket::is_event ( int event ) const { return (get_revents() & event); }
bool	Socket::is_opened ( void ) const { return (_opened); }

