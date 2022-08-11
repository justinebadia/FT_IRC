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
	
	t_socket		_socket;
	string			_buff[2]; // 0 == read and 1 == write
	bool			_socket_opened;
	int				_registration;
	

public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	Client( int fd );									// main constructor
	// Client( t_socket client_socket );				// main constructor
	Client( string nick ); 								// WARNING: TESTING PURPOSE constructor

	Client( const Client& other ); 						// copy constructor
	Client&	operator=( const Client& other ); 			// copy operator overload
	~Client( void ); 									// destructor
	

	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Client& rhs) const;


	/*-----------------------GETTERS----------------------*/

	const string&					get_nickname( void ) const;
	const string&					get_username( void ) const;
	string							get_hostname( void ) const;
	const string&					get_realname( void ) const;
	string							get_prefix( void );
	int								get_registration_flags( void ) const;

	static t_pollfd*				get_pollfd_array( void );
	t_pollfd&						get_pollfd( void );
	// t_addr6&						get_addr6_ref( void );
	// t_addr6							get_addr6_copy( void ) const;
	
	const int&						get_fd( void ) const;
	short							get_events( void ) const;
	short							get_revents( void ) const;
	
	string&							get_buff( u_int buff_i );
	const string&					get_source( void ) const;


	/*-----------------------SETTERS----------------------*/

	void	set_nickname( const string& nickname );
	void	set_username( const string& username );
	void	set_hostname( const string& hostname );
	void	set_realname( const string& realname );

	void	set_registration_flags( const e_registration& flag );


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
	

};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/

	std::ostream&	operator<<( std::ostream & o, const Client& obj );


} // namespace irc end bracket

#endif

