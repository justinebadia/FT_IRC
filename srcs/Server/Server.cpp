/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 18:29:18 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 18:46:23 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	// WARNING: implement more code here?
}

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

Server& Server::get_server( const unsigned int& port, const string password, bool exit ) // singleton
{
	static Server singleton(port, password, exit); // static singleton, calling the main server constructor

	return singleton;
}


Server::~Server( void ) // default destructor
{
	// more code ? not sure if delete will be used here to free the allocated memory
}


/*--------------------------------------GETTERS-----------------------------------------*/

bool					Server::get_exit_status( void ){ return _exit; }

const t_client_list&	Server::get_client_list( void ) { return _client_list; }

size_t					Server::get_client_count( void ) { return _client_list.size(); }

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

Client*				Server::get_client( string nickname )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_nickname() == nickname)
			return &(*it);
	}
	return NULL;
}


/*--------------------------------------SETTERS-----------------------------------------*/

void	Server::set_exit_status( bool true_signal ){ get_server()._exit = true_signal; }

