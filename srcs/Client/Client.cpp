/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 14:36:38 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 18:44:59 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

using namespace irc;

/* static initialization */
// t_client_list	Client::_client_list;
// size_t			Client::_client_count = 0;

Client::Client( int fd, t_addr6 addr )
{
	_socket.pollfd.fd = fd;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = true;
	_pending = 0;
	_socket.addr = addr;
	// _client_list.push_back(*this);
	// _client_count++;
	
}

Client::Client( string nick )
{
	_nickname = nick;
	_socket.pollfd.fd = 0;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = true;
	_pending = 0;
	// _client_list.push_back(*this);
	// _client_count++;
	
}

Client::Client( const Client& rhs )
{
	*this = rhs;
	
}

Client&	Client::operator=( const Client& rhs )
{
	set_nickname(rhs.get_nickname());
	set_username(rhs.get_username());
	_socket.pollfd.fd = rhs.get_fd();
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = rhs.is_opened();
	_pending = rhs.is_pending();
	_socket.addr = rhs.get_addr_copy();
	// _client_count++;
	return *this;
}

Client::~Client( void ) // destructor
{
	// freeaddrinfo( reinterpret_cast<struct addrinfo*>(&get_addr()) );

}

bool	Client::operator==( const Client& rhs) const
{
	if (rhs.get_nickname() == this->get_nickname())
		return true;
	return false;
}

/* static getters */
// const t_client_list&	Client::get_client_list ( void ) { return _client_list; }
// size_t					Client::get_client_count ( void ) { return _client_count; }
// Client *				Client::get_client ( int fd )
// {
// 	t_client_list::iterator it;

// 	for (it = _client_list.begin(); it != _client_list.end(); it++)
// 	{
// 		if ((*it).get_pollfd().fd == fd)
// 			return &(*it);
// 	}
// 	return NULL;
// }
// Client *				Client::get_client ( string nick )
// {
// 	t_client_list::iterator it;

// 	for (it = _client_list.begin(); it != _client_list.end(); it++)
// 	{
// 		if ((*it).get_nickname() == nick)
// 			return &(*it);
// 	}
// 	return NULL;
// }

/* getters */
t_pollfd&		Client::get_pollfd ( void ) { return _socket.pollfd; }
t_addr6&		Client::get_addr ( void ) { return _socket.addr; }
t_addr6			Client::get_addr_copy ( void ) const { return _socket.addr; }
const int&		Client::get_fd ( void ) const { return _socket.pollfd.fd; }
short			Client::get_events ( void ) const { return (_socket.pollfd.events); }
short			Client::get_revents ( void ) const { return (_socket.pollfd.revents); }
const string&	Client::get_nickname( void ) const { return _nickname; }
const string&	Client::get_username( void ) const { return _username; }
string&			Client::get_buff ( u_int buff_i ) 
{ 
	if (buff_i == BUFFIN)
		return _buff[0];
	if (buff_i == BUFFOUT)
		return _buff[1];
	return _buff[0];
}

/* setters */
void	Client::set_nickname ( const string& nick ) { _nickname = nick; }
void	Client::set_username ( const string& user ) { _username = user; }

/* utils */
bool	Client::is_event ( int event ) const { return (get_revents() & event); }
bool	Client::is_opened ( void ) const { return (_socket_opened); }
bool	Client::is_pending ( void ) const 
{
	 return _pending == (NICK_SET | USER_SET | PASS_SET); 
}

std::ostream&	irc::operator<<( std::ostream& o, const Client & obj )
{
	return o << obj.get_nickname();
}
