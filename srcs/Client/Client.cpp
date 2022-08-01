/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 14:36:38 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 16:37:27 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

using namespace irc;

Client::Client( int fd, t_addr6 addr )
{
	_socket.pollfd.fd = fd;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = true;
	_pending = 0;
	_socket.addr = addr;	
}

Client::Client( string nick )  // WARNING: TESTING PURPOSE constructor
{
	_nickname = nick;
	_socket.pollfd.fd = 0;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = true;
	_pending = 0;	
}

Client::Client( const Client& rhs ) // copy constructor
{
	*this = rhs;
}

Client&	Client::operator=( const Client& rhs ) // copy operator overload
{
	set_nickname(rhs.get_nickname());
	set_username(rhs.get_username());
	
	_socket.pollfd.fd = rhs.get_fd();
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = rhs.is_opened();
	
	_pending = rhs.is_pending();
	_socket.addr = rhs.get_addr_copy();
	return *this;
}

Client::~Client( void ) // destructor
{
	// freeaddrinfo( reinterpret_cast<struct addrinfo*>(&get_addr()) );
}

bool	Client::operator==( const Client& rhs) const
{
	return this->get_nickname() == rhs.get_nickname();
}

/* getters */
t_pollfd&		Client::get_pollfd ( void ) { return _socket.pollfd; }
t_addr6&		Client::get_addr ( void ) { return _socket.addr; }
t_addr6			Client::get_addr_copy ( void ) const { return _socket.addr; }
const int&		Client::get_fd ( void ) const { return _socket.pollfd.fd; }
short			Client::get_events ( void ) const { return (_socket.pollfd.events); }
short			Client::get_revents ( void ) const { return (_socket.pollfd.revents); }
const string&	Client::get_nickname( void ) const { return _nickname; }
const string&	Client::get_username( void ) const { return _username; }
const string&	Client::get_hostname( void ) const { return _nickname; } //TODO
const string&	Client::get_source( void ) const { return _nickname; } //TODO
// t_addr6&		Client::get_addr_ref( void ) { return _socket.addr; }
string&			Client::get_buff( u_int buff_i ) 
{ 
	if (buff_i == BUFFIN)
		return _buff[0];
	if (buff_i == BUFFOUT)
		return _buff[1];
	return _buff[0];
}

/*----------------------------------SETTERS------------------------------------*/

void	Client::set_nickname( const string& nick ) { _nickname = nick; }
void	Client::set_username( const string& user ) { _username = user; }
void	Client::set_pending_user_flags( const int flag ) { _pending |= flag; }

/*-----------------------------------UTILS-------------------------------------*/

bool	Client::is_event( int event ) const { return (get_revents() & event); }
bool	Client::is_opened( void ) const { return (_socket_opened); }
bool	Client::is_pending( void ) const { return _pending == (NICK_SET | USER_SET | PASS_SET); } // returns true if ALL THREE IS SET

std::ostream&	irc::operator<<( std::ostream& o, const Client & obj )
{
	return o << obj.get_nickname();
}
