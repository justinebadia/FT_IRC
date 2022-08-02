/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 15:17:09 by sfournie         ###   ########.fr       */
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

/*============================================================================*/
namespace irc {

class Client {

friend class Server; // WARNING: TESTING PURPOSE

enum e_pending
{
	NICK_SET = 1,
	USER_SET = 2,
	PASS_SET = 4
	// ALL_SET = 8  // The pending client is now a regular client 
};

private:

	Client( void ); 									// default constructor

	/*--------------------ATTRIBUTES---------------------*/
	
	string		_nickname;
	string		_username;
	t_socket	_socket;
	string		_buff[2]; // 0 == read and 1 == write
	bool		_socket_opened;
	int			_pending; //in registration
	
public:

	Client( int fd );									// main constructor
	// Client( t_socket client_socket );				// main constructor
	Client( string nick ); 								// WARNING: TESTING PURPOSE constructor

	Client( const Client& other ); 						// copy constructor
	Client&	operator=( const Client& other ); 			// copy operator overload
	~Client( void ); 									// destructor
	

	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Client& rhs) const;


	/*-----------------------GETTERS----------------------*/

	static t_pollfd*				get_pollfd_array( void );
	t_pollfd&						get_pollfd( void );
	t_addr&							get_addr( void );
	t_addr							get_addr_copy( void ) const;
	const int&						get_fd( void ) const;
	short							get_revents( void ) const;
	short							get_events( void ) const;
	const string&					get_buff( u_int buff_i );
	const string&					get_nickname( void ) const;
	const string&					get_username( void ) const;
	const string&					get_hostname( void ) const;
	const string&					get_source( void ) const;


	/*-----------------------SETTERS----------------------*/

	void	set_nickname( const string& nickname );
	void	set_username( const string& username );
	void	set_pending_user_flags( const int flag );

	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void	append_buff( u_int buff_i, const string& content );
	void	clear_buff( u_int buff_i );
	bool	is_event( int event ) const ;
	bool	is_opened( void ) const;
	bool	is_pending( void ) const;
	void	trim_buff( u_int buff_i, size_t len );
};


	/*----------------NON-MEMBER-FUNCTIONS----------------*/

	std::ostream&	operator<<( std::ostream & o, const Client& obj );


} // namespace irc end bracket

#endif

