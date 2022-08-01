/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 10:32:10 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <map>
#include <list>

#include "irc_define.hpp"
#include "typedef.hpp"
#include "struct.hpp"

using std::map;
using std::string;

namespace irc {

class Client {

enum e_pending
{
	NICK_SET = 1,
	USER_SET = 2,
	PASS_SET = 4
};

private:	
	string		_nickname;
	string		_username;
	t_socket	_socket;
	string		_buff[2];
	bool		_socket_opened;
	int			_pending; //in registration
	
	static map<string, Client> _client_map;

	Client( void );
public:
	Client( int fd, t_addr6 addr );
	Client( string nick ); // WARNING: TESTING PURPOSE
	Client( const Client& other );
	Client&	operator=( const Client& other );
	~Client( void );
	
	bool	operator==( const Client& rhs) const;

	/* Getters */
	static t_pollfd*				get_pollfd_array ( void );
	t_pollfd&						get_pollfd ( void );
	t_addr6&						get_addr ( void );
	t_addr6							get_addr_copy ( void ) const;
	const int&						get_fd ( void ) const;
	short							get_revents ( void ) const;
	short							get_events ( void ) const;
	string&							get_buff ( u_int buff_i );
	const string &					get_nickname( void ) const;
	const string &					get_username( void ) const;

	/* Setters */
	void	set_nickname ( const string& nick );
	void	set_username ( const string& user );

	/* Utils */
	bool	is_event ( int event ) const ;
	bool	is_opened ( void ) const;
	bool	is_pending ( void ) const;
};

std::ostream&	operator<<( std::ostream & o, const Client& obj );

} // namespace irc end scope

#endif

/* notes:
user status:
->pending (connected to the server, but doesn't have a nick yet);
->ban 
->kick
->
*/
