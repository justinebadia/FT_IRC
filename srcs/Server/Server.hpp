/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/08 10:23:27 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <utility>
#include <signal.h>
#include "irc_define.hpp"
#include <signal.h>
#include <exception>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../Client/Client.hpp"
#include "Database.hpp"
#include "Message.hpp"
#include "CommandManager.hpp"
#include "typedef.hpp"

#define	HOSTNAME "127.0.0.1"	// a.k.a. "localhost" alias
#define PORT 6667
 
class Channel;

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class CommandManager;

class Server {

private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	Server( void );										// default constructor
	Server( const Server& other );						// copy constructor
	Server& operator=( const Server& other );			// copy operator overload 

	
	/*--------------------ATTRIBUTES---------------------*/

	Database			_database;
	t_socket			_server_socket; // t_pollfd pollfd, t_addr addr6; 
	const string		_server_name;
	const unsigned int	_port;
	const string		_password;
	bool				_exit;

	t_client_list		_client_list;
	t_command_map		_command_map;
	t_reply_map			_reply_map;

	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	

public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	Server( const unsigned int& port, const string password, bool exit );	// main constructor
	~Server( void );														// default destructor


private:

	/*---------------PRIVATE-MEMBER-FUNCTIONS---------------*/

	void	_process_client_pollerr( const t_pollfd& pollfd );
	void	_process_client_pollin( const t_pollfd& pollfd );
	void	_process_client_pollout( const t_pollfd& pollfd );

	
public:

	/*-----------------------GETTERS----------------------*/
	
	static Server&			get_server( const unsigned int& port = 0, const string password = "", bool exit = false ); // singleton

	const t_socket&			get_socket( void ) const;
	const string&			get_name( void ) const;
	const unsigned int		get_port( void ) const;
	Database*		 		get_database( void );
	const string&			get_password( void ) const;
	bool					get_exit_status( void ) const;

	t_pollfd&				get_pollfd( void );
	t_addr6&				get_addr6( void );
	const int&				get_fd( void ) const;

	// [Message related getters]
	
	t_pollfd*				get_pollfd_array( void );

	
	/*-----------------------SETTERS----------------------*/

	void	set_fd( int fd );
	void	set_exit( bool status );
	void	set_signal_ctrl_c( void );
	

	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/
	
	int			run_server( void );
	void		init_server( void );

	t_pollfd*	poll_sockets( void );
	void		process_connections( const t_pollfd& pollfd );
	void		process_clients( const t_pollfd* pollfd_array, size_t size );

	/*---------------NESTED-CLASS-EXCEPTIONS---------------*/
	
	class SocketErrorException : public std::exception
	{
		public: virtual const char* what() const throw();
	};

	class SetsockoptErrorException : public std::exception
	{
		public: virtual const char* what() const throw();
	};
	
	class BindErrorException : public std::exception
	{
		public: virtual const char* what() const throw();
	};
	
	class ListenErrorException : public std::exception
	{
		public: virtual const char* what() const throw();
	};

};



} // namespace irc end bracket

void	set_exit_true( int signal ); // WARNING To be moved

#endif
