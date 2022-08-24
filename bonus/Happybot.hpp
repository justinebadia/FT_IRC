#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>

#define	HAPPY	1
#define GRUMPY	0

using std::vector;
using std::string;

/*============================================================================*/
namespace irc {

class Happybot 
{
public:
	typedef struct s_info
	{
		string	server_ip;
		string	server_port;
		string	server_pass;
		string	nick;
		string	user;
		string	realname;

		s_info() { }
	} t_info;

private:

	Happybot( const Happybot& ) {  }; 					// copy constructor

	/*--------------------ATTRIBUTES---------------------*/
	vector<string>	_happy_thoughts;
	vector<string>	_grumpy_thoughts;
	int				_mood;
	string			_name;
	string			_user;
	string			_realname;
	struct pollfd	_pollfd[1];
	string			buffin;
	string			buffout;
	
	void	_init_Happybot( void );
	void	_init_happy_thoughts( void );
	void	_init_grumpy_thoughts( void );

public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	Happybot( void ); 
	Happybot( string name ); 
	~Happybot( void ); 								// destructor
	Happybot&	operator=( const Happybot& ); 		// copy operator overload

	/*--------------------BOT-FUNCTIONS--------------------*/

	void	set_as_grumpy( void );
	void	set_as_happy( void );

	string	get_random_thought( void );
	string	get_random_grumpy_thought( void );
	string	get_random_happy_thought( void );

	int		connect_to_server( string hostname, int port, string password );
	int		run_bot( void );
	void	process_buffin( void );

	void	join_happy_world( void );
	void	send_love_to_everybody( void );
	void	send_love_to_someone( string target_nick );
	void	send_love_to_channel( string channel_name );
	
};

} // namespace irc end bracket
