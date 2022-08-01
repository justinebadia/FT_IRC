/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 18:29:18 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 15:43:09 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp" // includes: <string><list><map><vector><signal.h> "typedef.hpp" "irc_define.hpp" "Client.hpp"

using namespace irc;

Server::Server( void ) : _port(PORT), _password("") {} // default constructor [PRIVATE]


Server::Server( const Server& other ) // copy constructor + initialization list [PRIVATE]
	: _server_socket(other._server_socket)
	, _port(other._port)
	, _password(other._password)
	, _exit(other._exit)
{ 
	*this = other;
}


Server&	Server::operator=( const Server& other ) // copy operator overload [PRIVATE]
{
	// this->_database = other._database; // use stl algorithm to copy the pairs in the list
	// return *this;
}


Server::Server( const unsigned int& port, const string password, bool exit ) // main server constructor + initialization list
	: _server_socket(0)
	, _port(PORT) // 6667 defined in Server.hpp
	, _password(password)
	, _exit(false)
{
	// more code
}

Server::~Server( void ) // default destructor
{
	// more code ? not sure if delete will be used here to free the allocated memory
}


/*-----------------------------------GETTERS---------------------------------*/

Server& Server::get_server( const unsigned int& port, const string password, bool exit ) // singleton
{
	static Server singleton(port, password, exit); // static singleton declared on the stack, calling the main server constructor

	return singleton;
}

t_socket				Server::get_server_socket( void ){ return _server_socket; }

const unsigned int		Server::get_server_port( void ){ return _port; }

const string			Server::get_server_password( void ){ return _password; }

bool					Server::get_exit_status( void ){ return _exit; }

const t_client_list&	Server::get_client_list ( void ) { return _client_list; }

size_t					Server::get_client_count ( void ) { return _client_list.size(); }

Client*					Server::get_client( int fd )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_pollfd().fd == fd)
			return &(*it);
	}
	return NULL;
}

Client*				Server::get_client( const string nickname )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_nickname() == nickname)
			return &(*it);
	}
	return NULL;
}


/*-----------------------------------SETTERS---------------------------------*/

void	Server::set_signal_ctrl_c( const Server& server )
{
	signal(SIGINT, &server.set_exit_true);
}

void	Server::set_exit_true( int signal )
{
	(void)signal;

	// more code to free allocated memory here? or after the main loop?

	get_server()._exit = true; 
}



/*-------------------------OTHER-MEMBER-FUNCTIONS----------------------------*/

void	Server::add_client( const Client& client )
{
	_client_list.push_back(client);
}

void	Server::remove_client( const string& nickname )
{
	Client* c;

	c = get_client(nickname);
	if (c != NULL)
		_client_list.remove(*c); //WARNING: TESTING PURPOSE
}

