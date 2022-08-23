#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>

#include <list>

#include "irc_define.hpp"
#include "typedef.hpp"

using std::map;
using std::string;

/*============================================================================*/
namespace irc {

class Client {

friend class Server; // WARNING: TESTING PURPOSE

public:

	/*--------------------TYPEDEF-&-ENUM-------------------*/
	
	enum e_registration
	{
		NONE_SET = 0,
		NICK_SET = 1,
		USER_SET = 2,
		PASS_SET = 4,
		COMPLETE = 8 // The pending client is now a regular client 
	};


private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	Client( void ); 									// default constructor


	/*--------------------ATTRIBUTES---------------------*/
	
	string			_nickname;
	string			_username;
	string			_hostname;
	string 			_realname;
	string 			_client_ip;
	bool			_to_be_killed;
	bool			_operator;
	
	t_socket		_socket;
	string			_buff[2]; // 0 == read and 1 == write
	bool			_socket_opened;
	int				_registration;
	time_t			_last_read;
	
	void	_set_operator( bool value );
	void	_init_client( void );

public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	Client( int fd );									// main constructor
	// Client( t_socket client_socket );				// main constructor
	Client( int fd, string hostname );
	Client( const Client& rhs ); 						// copy constructor
	Client&	operator=( const Client& rhs ); 			// copy operator overload
	~Client( void ); 									// destructor
	

	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Client& rhs) const;


	/*-----------------------GETTERS----------------------*/

	const string&					get_nickname( void ) const;
	const string&					get_username( void ) const;
	string							get_hostname( void ) const;
	const string&					get_realname( void ) const;
	const string& 					get_client_ip( void ) const;
	bool 							get_to_be_killed( void ) const;
	string							get_prefix( void );
	string							get_full_id( void );
	int								get_registration_flags( void ) const;
	const time_t&					get_last_read( void ) const;

	t_pollfd&						get_pollfd( void );
	t_addr6&						get_addr6_ref( void );
	t_addr6							get_addr6_copy( void ) const;
	
	const int&						get_fd( void ) const;
	short							get_events( void ) const;
	short							get_revents( void ) const;
	
	string&							get_buff( u_int buff_i );
	const string&					get_source( void ) const;


	/*-----------------------SETTERS----------------------*/

	void	set_fd( const int& fd );
	void	set_ip( const string& ip );
	void	set_nickname( const string& nickname );
	void	set_username( const string& username );
	void	set_hostname( const string& hostname );
	void	set_realname( const string& realname );
	void	set_last_read_to_now( void );

	void	set_registration_flags( const e_registration& flag );
	void	set_to_be_killed( bool setting );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void	append_buff( u_int buff_i, const string& content );
	void	clear_buff( u_int buff_i );
	void	trim_buff( u_int buff_i, size_t len );

	bool	is_event( int event ) const ;
	bool	is_opened( void ) const;
	bool	is_nickname_set( void ) const;
	bool	is_username_set( void ) const;
	bool	is_password_validated( void ) const;
	bool	is_registered( void ) const;
	bool	is_operator( void ) const;
	
	const string 	grab_client_ip_adress( void );

};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/

	std::ostream&	operator<<( std::ostream & o, const Client& obj );


} // namespace irc end bracket

#endif

