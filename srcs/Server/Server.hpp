/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/02 14:07:47 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include <signal.h> // SIGINT == control-C
#include "irc_define.hpp"
#include <signal.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../Client/Client.hpp"
#include "Message.hpp"
#include "typedef.hpp"


#define PORT 6667 // irc server port number
 
class Channel;

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
	const string		_server_name;
	bool				_exit;

	t_client_list		_client_list;
	t_command_map		_command_map;
	t_reply_map			_reply_map;

	std::list<std::pair<Client* , Channel* > >	_database;
	
	struct addrinfo		_hints;
	struct addrinfo*	_res;
	struct in_addr		_addr;
	struct sockaddr_in	_address;

	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	
	
public:

	Server( const unsigned int& port, const string password, bool exit );	// main constructor
	~Server( void );														// default destructor
	
	/*-----------------------GETTERS----------------------*/
	
	static Server&	get_server( const unsigned int& port = 0, const string password = "", bool exit = false ); // singleton

	t_socket				get_server_socket( void );
	const unsigned int		get_server_port( void );
	const string			get_server_password( void );
	bool					get_exit_status( void );

	const t_client_list&	get_client_list( void );
	Client*					get_client( int fd );
	Client*					get_client( const string nickname );
	size_t					get_client_count( void );
	t_pollfd*				get_pollfd_array( void );

	const string&			get_server_name( void ); //TODO
	t_cmd_function_ptr		get_command_ptr( string name );
	t_reply_function_ptr	get_reply_ptr( int code );

	
	/*-----------------------SETTERS----------------------*/
	
	void	set_signal_ctrl_c( const Server& server );
	void	set_exit_true( int signal ); 


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/
	
	void	add_client( const Client& client );
	void	remove_client( const string& nickname );
	void	init_command_map( void );
	void	init_reply_map( void );

};

} // namespace irc end bracket

#endif
