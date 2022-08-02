/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 18:29:18 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 14:25:41 by tshimoda         ###   ########.fr       */
/*   Updated: 2022/08/02 08:51:50 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp" // includes: <string><list><map><vector><utility><signal.h> "irc_define.hpp"
					  // <arpa/inet.h><netinet/in.h><sys/types.h><sys/socket.h>
					  // "../Client/Client.hpp" "Message.hpp" "typedef.hpp"

#include "commands.hpp"
#include "replies.hpp"
#include "numeric_replies.hpp"
#include <iostream>

using namespace irc;

Server::Server( void ) : _port(PORT), _password(""){}		// default constructor [PRIVATE]

Server::Server( const Server& other ) 						// copy constructor [PRIVATE]
	: _server_socket(other._server_socket)
	, _server_name(other._server_name);
	, _port(other._port)
	, _password(other._password)
	, _exit(other._exit)
	{ *this = other; }

Server&	Server::operator=( const Server& other ){}			// copy operator overload [PRIVATE]


Server::Server( const unsigned int& port, const string password, bool exit ) // main server constructor
	: _server_name(HOSTNAME)	// 127.0.0.1 
	, _port(PORT)				// 6667
	, _password(password)
	, _exit(false)
{
	if (_server_socket.pollfd.fd = socket(AF_INET6, SOCK_STREAM, 0) == FAIL)
	{
		std::cout << "Error: socket()" << std::endl;
		throw Server::SocketErrorException();
	}
	_server_socket.pollfd.events = 0;
	_server_socket.pollfd.revents = 0;

	init_command_map();
	init_reply_map();
}


Server::~Server( void )										// default destructor
{}


/*---------------------------------GETTERS-----------------------------------*/

Server&				Server::get_server( const unsigned int& port, const string password, bool exit ) // singleton
{
	static Server singleton(port, password, exit); // static singleton declared on the stack, call the main constructor
	return singleton;
}

const t_socket&			Server::get_server_socket( void ) const { return _server_socket; }
const string&			Server::get_server_name( void ) const { return _server_name; }
const unsigned int		Server::get_port( void ) const { return _port; }
const string&			Server::get_password( void ) const { return _password; }
const bool				Server::get_exit_status( void ) const { return _exit; }
const int				Server::get_pollfd_fd( void ) const { return _server_socket.pollfd.fd; }


// [Client related getters]

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

Client*					Server::get_client ( string nickname )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_nickname() == nickname)
			return &(*it);
	}
	return NULL;
}


// [Message related getters]

t_cmd_function_ptr		Server::get_command_ptr( string name )
{
	t_command_map::iterator it;

	for (it = _command_map.begin(); it != _command_map.end(); it++)
	{
		if ((*it).first == name)
			return (*it).second;
	}
	return NULL;
}

t_reply_function_ptr	Server::get_reply_ptr( int code )
{
	t_reply_map::iterator it;

	for (it = _reply_map.begin(); it != _reply_map.end(); it++)
	{
		if ((*it).first == code)
			return (*it).second;
	}
	return NULL;
}

// ====== INSERT HERE =======  get_pollfd_array() !!!
// ====== INSERT HERE =======  get_pollfd_array() !!!
// ====== INSERT HERE =======  get_pollfd_array() !!!


/*---------------------------------SETTERS-----------------------------------*/

void	Server::signal_ctrl_c( void )
{
	signal(SIGINT, &set_exit_true( int signal );
}

void	Server::set_exit_true( int signal )
{
	(void)signal;
	_exit = true;
}

/*--------------------------OTHER-MEMBER-FUNCTIONS---------------------------*/

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


/*-------------------------NESTED-CLASS-EXCEPTIONS--------------------------*/

const char*	Server::SocketErrorException::what() const throw()
{
	return "Error: creation of server socket() ";
}

