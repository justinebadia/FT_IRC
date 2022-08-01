/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 18:29:18 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 17:10:10 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp" // including : <string><list><map><vector>
#include "commands.hpp"
#include "replies.hpp"
#include "numeric_replies.hpp"

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
	init_command_map();
	init_reply_map();
	// more code
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
	static Server singleton(port, password, exit); // static singleton declared on the stack, calling the main server constructor

	return singleton;
}


Server::~Server( void ) // default destructor
{
	// more code ? not sure if delete will be used here to free the allocated memory
}


bool	Server::get_exit_status( void ){ return _exit; }

const t_client_list&	Server::get_client_list ( void ) { return _client_list; }
size_t					Server::get_client_count ( void ) { return _client_list.size(); }
Client*					Server::get_client ( int fd )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_pollfd().fd == fd)
			return &(*it);
	}
	return NULL;
}
Client*	Server::get_client ( string nickname )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_nickname() == nickname)
			return &(*it);
	}
	return NULL;
}

t_cmd_function_ptr		Server::get_command_ptr ( string name )
{
	t_command_map::iterator it;

	for (it = _command_map.begin(); it != _command_map.end(); it++)
	{
		if ((*it).first == name)
			return (*it).second;
	}
	return NULL;
}

t_reply_function_ptr	Server::get_reply_ptr ( int code )
{
	t_reply_map::iterator it;

	for (it = _reply_map.begin(); it != _reply_map.end(); it++)
	{
		if ((*it).first == code)
			return (*it).second;
	}
	return NULL;
}

void	Server::set_exit_status( bool true_signal ){ get_server()._exit = true_signal; }

void	Server::init_command_map( void )
{
	_command_map.insert(std::make_pair(string("NICK"), cmd_nick));
	//_command_map.insert(std::make_pair(string("NOM_DE_COMMANDE"), cmd_join));

}

void	Server::init_reply_map( void )
{
	// _reply_map.insert(std::make_pair(ERR_NONICKNAMEGIVEN, err_nonicknamegiven));
	_reply_map.insert(std::make_pair(ERR_ERRONEUSNICKNAME, err_erroneusnickname));
	_reply_map.insert(std::make_pair(ERR_NICKNAMEINUSE, err_nicknameinuse));
	// _reply_map.insert(std::make_pair(ERR_NICKCOLLISION, err_nickcollision));
	_reply_map.insert(std::make_pair(ERR_NOSUCHSERVER, err_nosuchserver));
	// _reply_map.insert(std::make_pair(ERR_USERDISABLED, *err_userdisabled));
	// _reply_map.insert(std::make_pair(ERR_NOUSERS, rpl_nousers));
	// _reply_map.insert(std::make_pair(RPL_USERSSTART, rpl_usersstart));
	// _reply_map.insert(std::make_pair(RPL_ENDOFUSERS, rpl_endofusers));
	_reply_map.insert(std::make_pair(ERR_NEEDMOREPARAMS, err_needmoreparams));
	_reply_map.insert(std::make_pair(ERR_ALREADYREGISTERED, err_alreadyregistered));

	//_command_map.insert(std::make_pair(string("NOM_DE_COMMANDE"), cmd_join));

}

const string& Server::get_server_name( void )
{

}
