/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 14:50:27 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <map>
#include <list>

#include "irc_define.hpp"
#include "typedef.hpp"

using std::map;
using std::string;

namespace irc {

class Client {

private:
	static t_client_list	_client_list;
	static size_t			_client_count;
	
	string		_nickname;
	string		_username;
	t_pollfd	_pollfd;
	t_addr6		_address;
	string		_buff[2];
	bool		_opened;
	
	static map<string, Client> _client_map;

	Client( void ); // default constructor
	Client( const Client& other ); // copy constructor
	Client&	operator=( const Client& other ); // copy operator overload
public:
	Client( size_fd fd );
	~Client( void ); // destructor

	
	/* Getters */
	static const t_client_list	get_client_list ( void );
	static t_pollfd*			get_pollfd_array ( void );
	static size_t				get_client_count ( void );
	t_pollfd&					get_pollfd ( void );
	t_addr6&					get_addr ( void );
	const size_fd&				get_fd ( void ) const;
	short						get_revents ( void ) const;
	short						get_events ( void ) const;
	string&						get_buff ( u_int buff_i );
	const string &				get_nickname( void ) const;
	const string &				get_username( void ) const;

	/* Setters */
	void	set_events ( const short & fde );
	void	set_addr ( const t_addr6 & addr );
	void	set_pollfd ( t_pollfd * pfd );
	void	set_fd ( const size_fd & fd );

	/* Utils */
	bool	is_event ( int event ) const ;
	bool	is_opened ( void ) const;
};

std::ostream	&operator << ( std::ostream & o, const Client& obj );

} // namespace irc end scope

#endif

/* notes:
user status:
->pending (connected to the server, but doesn't have a nick yet);
->ban 
->kick
->
*/
