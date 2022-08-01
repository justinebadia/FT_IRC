/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 18:41:58 by sfournie         ###   ########.fr       */
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

using std::map;
using std::string;

namespace irc {

class Client {

friend class Server; // WARNING: TESTING PURPOSE

enum e_pending
{
	NICK_SET = 1,
	USER_SET = 2,
	PASS_SET = 4,
	// ALL_SET = 8  // The pending client is now a regular client 
};

private:

	Client( void ); // default constructor

	// static t_client_list	_client_list;
	// static size_t			_client_count;
	
	string		_nickname;
	string		_username;
	t_socket	_socket;
	string		_buff[2]; // 0 == read and 1 == write
	bool		_socket_opened;
	int			_pending; //in registration
	
public:

	Client( int fd, t_addr6 addr ); // main constructor
	Client( string nick ); // WARNING: TESTING PURPOSE

	Client( const Client& other ); // copy constructor
	Client&	operator=( const Client& other ); // copy operator overload
	~Client( void ); // destructor
	
	bool	operator==( const Client& rhs) const;

	/* Getters */
	// static const t_client_list &	get_client_list( void );
	// static Client*					get_client( int fd );
	// static Client*					get_client( string nick );
	static t_pollfd*				get_pollfd_array( void );
	// static size_t					get_client_count( void );
	t_pollfd&						get_pollfd( void );
	t_addr6&						get_addr( void );
	t_addr6							get_addr_copy( void ) const;
	const int&						get_fd( void ) const;
	short							get_events( void ) const;
	short							get_revents( void ) const;
	string&							get_buff( u_int buff_i );
	const string&					get_nickname( void ) const;
	const string&					get_username( void ) const;

	/* Setters */
	void	set_nickname( const string& nickname );
	void	set_username( const string& username );
	void	set_pending_user_flags( const int flag );

	/* Utils */
	void	append_buff( u_int buff_i );
	void	clear_buff( u_int buff_i );
	bool	is_event( int event ) const ;
	bool	is_opened( void ) const;
	bool	is_pending( void ) const;
};

std::ostream&	operator<<( std::ostream & o, const Client& obj );

} // namespace irc end scope

#endif

