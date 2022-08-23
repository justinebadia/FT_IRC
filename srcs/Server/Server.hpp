
#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <time.h>
#include <string>
#include <utility>
#include <exception>
#include <vector>

#include "Database.hpp"
#include "irc_define.hpp"
#include "typedef.hpp"

#define PORT 6667
#define CLIENT_TIMEOUT	2400	// seconds
#define PING_INTERVAL	1200	// seconds
 
class Channel;

using std::string;
using std::vector;

/*============================================================================*/ 
namespace irc {

class CommandManager;

class Server {

private:

	class Operator
	{
	public:
		const string	name;
		const string	password;
		int				client_fd;

		Operator( const string& name, const string& pass ) : name(name), password(pass), client_fd(-1) {  }
	};
	typedef vector<Operator>	t_operator_vect;
	
	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	Server( void );										// default constructor
	Server( const Server& other );						// copy constructor
	Server& operator=( const Server& other );			// copy operator overload 

	
	/*--------------------ATTRIBUTES---------------------*/

	Database			_database;
	t_socket			_server_socket; // t_pollfd pollfd, t_addr addr6; 
	const string		_server_ip;
	const string		_server_name;
	const unsigned int	_port;
	const string		_password;
	bool				_exit;

	t_client_list		_client_list;
	t_command_map		_command_map;
	t_reply_map			_reply_map;
	t_operator_vect		_operator_vect;

	time_t				_last_ping;
	static int			log_level;

	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	

public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	Server( const unsigned int& port, const string password, bool exit );	// main constructor
	~Server( void );														// default destructor


private:

	/*---------------PRIVATE-MEMBER-FUNCTIONS---------------*/

	void		_init_server( void );
	void		_kill_server( void );
	void		_check_for_kills( void );
	void		_check_for_timeouts( void );

	void		_init_operators( void );
	void		_clean_operators( void );
	

	t_pollfd*	_poll_sockets( void );
	void		_process_connections( const t_pollfd& pollfd );
	void		_process_clients( const t_pollfd* pollfd_array, size_t size );
	void		_process_client_pollerr( const t_pollfd& pollfd );
	void		_process_client_pollin( const t_pollfd& pollfd );
	void		_read_client_socket( const int& fd, char** buffer, ssize_t* bytes );
	void		_process_client_pollout( const t_pollfd& pollfd );

public:

	/*-----------------------GETTERS----------------------*/
	
	static Server&			get_server( const unsigned int& port = 0, const string password = "", bool exit = false ); // singleton

	const t_socket&			get_socket( void ) const;
	const string&			get_name( void ) const;
	string					get_prefix( void ) const;
	const unsigned int&		get_port( void ) const;
	Database*		 		get_database( void );
	const string&			get_password( void ) const;
	bool					get_exit_status( void ) const;
	const string&			get_server_ip ( void ) const;

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
	void		check_registration( Client* client );
	void		disconnect_client( const int& fd );
	void		disconnect_all_clients( void );

	int			attempt_client_as_operator( Client& client, const string& oper_name, const string& oper_pass );
	bool		is_client_operator( Client* client );
	bool		is_client_operator( const string& nickname );
	bool		is_client_operator( const int& fd );

	const string grab_ip_address( void );

	static void	log( const string& msg );
	static void	log_error( const string& msg );

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
