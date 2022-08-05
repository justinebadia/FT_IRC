

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>

#include <netdb.h>
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
	string		_hostname;
	string 		_realname;
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
	t_addr6&						get_addr6_ref( void );
	t_addr6							get_addr6_copy( void ) const;
	const int&						get_fd( void ) const;
	short							get_revents( void ) const;
	short							get_events( void ) const;
	const string&					get_buff( u_int buff_i );
	const string&					get_nickname( void ) const;
	const string&					get_username( void ) const;
	string							get_hostname( void ) const;
	const string&					get_realname( void ) const;
	const string&					get_source( void ) const;


	/*-----------------------SETTERS----------------------*/

	void	set_nickname( const string& nickname );
	void	set_username( const string& username );
	void	set_pending_user_flags( const int flag );
	void	set_realname( const string& realname );

	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void 	execute_commands( void );
	void	append_buff( u_int buff_i, const string& content );
	void	clear_buff( u_int buff_i );
	void	trim_buff( u_int buff_i, size_t len );

	bool	is_event( int event ) const ;
	bool	is_opened( void ) const;
	bool	is_pending( void ) const;
	
};


	/*----------------NON-MEMBER-FUNCTIONS----------------*/

	std::ostream&	operator<<( std::ostream & o, const Client& obj );


} // namespace irc end bracket

#endif

