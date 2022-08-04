/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 14:36:38 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/04 17:23:38 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server/Server.hpp"
#include "Message.hpp"
#include "Client.hpp"

using namespace irc;

Client::Client( int fd )
{
	_socket.pollfd.fd = fd;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = true;
	_pending = 0;
	// _socket.addr = addr;	
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
	// _socket.addr = rhs.get_addr_copy();
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
t_addr&			Client::get_addr ( void ) { return _socket.addr; }
t_addr			Client::get_addr_copy ( void ) const { return _socket.addr; }
const int&		Client::get_fd ( void ) const { return _socket.pollfd.fd; }
short			Client::get_events ( void ) const { return (_socket.pollfd.events); }
short			Client::get_revents ( void ) const { return (_socket.pollfd.revents); }
const string&	Client::get_nickname( void ) const { return _nickname; }
const string&	Client::get_username( void ) const { return _username; }
const string&	Client::get_realname( void ) const { return _realname; }
const string&	Client::get_source( void ) const { return _nickname; } //TODO
// t_addr6&		Client::get_addr_ref( void ) { return _socket.addr; }

string	Client::get_hostname( void ) const {return (_nickname + "!" + _username + "@127.0.0.1"); } 
 
const string&	Client::get_buff( u_int buff_i )
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
void	Client::set_realname( const string& realname ) { _realname = realname; }
void	Client::set_pending_user_flags( const int flag ) { _pending |= flag; }

/*-----------------------------------UTILS-------------------------------------*/

void	Client::execute_commands( void )
{
	size_t start	= 0;
	size_t last		= 0;
	size_t next		= 0;
	size_t len		= _buff[BUFFIN].length();
	Message	msg(this);
	t_cmd_function_ptr command;

	std::cout << "BUFFIN : " << _buff[BUFFIN] << " FIND = " << _buff[BUFFIN].find("\r\n", start) << std::endl;
	while ((next = _buff[BUFFIN].find("\r\n", start)) != string::npos)
	{
		msg = Message(this);
		msg.append_in(_buff[BUFFIN].substr(start, next - start));
		std::cout << "le msg est : " << msg.get_message_in() << std::endl;
		command = Server::get_server().get_command_ptr(msg[0]);
		if (command)
		{
			command(msg);
			this->append_buff(BUFFOUT, msg.get_message_out());
		}
		msg.clear_all();
		start = next + 2;
		if (start >= len)
			return ;
	}
	return ;
}

// void	Client::execute_commands( void ) //VRAIMENT PAS SUR !!!
// {

// }

void	Client::append_buff( u_int buff_i, const string& content )
{
	if (buff_i == BUFFIN)
		_buff[0].append(content);
	if (buff_i == BUFFOUT)
		_buff[1].append(content);
}

void	Client::clear_buff( u_int buff_i )
{
	if (buff_i == BUFFIN)
		_buff[0].clear();
	if (buff_i == BUFFOUT)
		_buff[1].clear();
}

void	Client::trim_buff( u_int buff_i, size_t len )
{
	string	*buff = NULL;

	if (buff_i == BUFFIN)
		buff = &buff[0];
	if (buff_i == BUFFOUT)
		buff = &buff[1];
	if (!buff)
		return ;
	*buff = buff->substr(0, len);
}

bool	Client::is_event( int event ) const { return (get_revents() & event); }
bool	Client::is_opened( void ) const { return (_socket_opened); }
bool	Client::is_pending( void ) const { return _pending == (NICK_SET | USER_SET | PASS_SET); } // returns true if ALL THREE IS SET

std::ostream&	irc::operator<<( std::ostream& o, const Client & obj )
{
	return o << obj.get_nickname();
}
