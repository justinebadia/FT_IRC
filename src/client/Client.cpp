/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 14:36:38 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 14:50:15 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_define.hpp"
#include "typedef.hpp"
#include "Client.hpp"

using namespace irc;

/* static initialization */
t_client_list	_client_list;
size_t			_client_count = 0;

Client::~Client( void ) // destructor
{}

/* static getters */
const t_client_list&	get_client_list ( void ) { return _client_list; }
size_t					get_client_count ( void ) { return _client_count; }

/* getters */
t_pollfd&			Client::get_pollfd ( void ) { return _pollfd; }
t_addr6&			Client::get_addr ( void ) { return _address; }
const size_fd&		Client::get_fd ( void ) const { return _pollfd.fd; }
short				Client::get_events ( void ) const { return (_pollfd.events); }
short				Client::get_revents ( void ) const { return (_pollfd.revents); }
const string&		Client::get_nickname( void ) const { return _nickname; }
const string&		Client::get_username( void ) const { return _username; }
string&				Client::get_buff ( u_int buff_i ) 
{ 
	if (buff_i == BUFFIN)
		return _buff[BUFFIN];
	if (buff_i == BUFFOUT)
		return _buff[BUFFOUT];
	return _buff[BUFFIN];
}

/* setters */
void	Client::set_pollfd ( t_pollfd * pfd ) { _pollfd = pfd; }
void	Client::set_fd ( const size_fd & fd ) { get_pollfd().fd = fd; }
void	Client::set_events ( const short & fde )	{ get_pollfd().events = fde; }

/* utils */
bool	Client::is_event ( int event ) const { return (get_revents() & event); }
bool	Client::is_opened ( void ) const { return (_opened); }



std::ostream&	operator<<( std::ostream & o, Client const & obj )
{
	return o;
}
