/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/01 16:31:56 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <signal.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../includes/typedef.hpp"
#include "../../includes/irc_define.hpp"
#include "../Client/Client.hpp"

#define PORT 6667 // irc server port number
 
class Channel;
class Message;

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Server {

private:

	Server( void );										// default constructor
	Server( const Server& other );						// copy constructor
	Server& operator=( const Server& other );			// copy operator overload 

	
	/*--------------------ATTRIBUTES---------------------*/
	t_socket			_server_socket; // this struct is defined includes/typdef.hpp
	const unsigned int	_port;
	const string		_password;
	bool				_exit;
	
	struct addrinfo		_hints;
	struct addrinfo*	_res;
	struct in_addr		_addr;
	struct sockaddr_in	_address;

	t_client_list						_client_list;
	list<pair<Client* , Channel* > >	_database;
	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	
	
public:

	Server( const unsigned int& port, const string password, bool exit );	// main constructor
	~Server( void );														// default destructor
	
	/*----------------------GETTERS----------------------*/
	
	static Server&	get_server( c onst unsigned int& port = 0, const string password = "", bool exit = false ); // singleton

	t_socket				get_server_socket( void );
	const unsigned int		get_server_port( void );
	const string			get_server_password( void );
	bool					get_exit_status( void );

	const t_client_list&	get_client_list( void );
	Client*					get_client( int fd );
	Client*					get_client( const string nickname );
	size_t					get_client_count( void );
	t_pollfd*				get_pollfd_array( void );


	/*----------------------SETTERS----------------------*/
	
	void	set_signal_ctrl_c( const Server& server );
	void	set_exit_true( int signal ); 

	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/
	
	void	add_client( const Client& client );
	void	remove_client( const string& nickname );

};

} // namespace irc end bracket

#endif
