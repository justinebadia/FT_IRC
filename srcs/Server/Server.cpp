#include "Server.hpp" // including : <string><list><map><vector>


Server::Server( void ){} // default constructor [PRIVATE]


Server::Server( const Server& other ) // copy constructor + initialization list [PRIVATE]
	: _server_socket(other._server_socket)
	, _port(other._port)
	, _password(other._password)
	, _exit(other._exit)
	{ *this = other; }


Server&	irc::Server::operator( const Server& other ) // copy operator overload [PRIVATE]
{
	// this->_database = other._database; // use stl algorithm to copy the pairs in the list
}


Server::Server( const unsigned int& port, const std::string password, bool exit ) // main server constructor + initialization list
	: _server_socket(0)
	, _port(PORT) // 6667 defined in Server.hpp
	, _password(password)
	, _exit(false)
{
	// more code
}


Server::~Server( void ) // default destructor
{
	// more code ? not sure if delete will be used here to free the allocated memory
}


bool	irc::Server::get_exit_status( void ){ return this->_exit; }
