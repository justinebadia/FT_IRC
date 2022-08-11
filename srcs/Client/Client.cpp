#include "../Server/Server.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "../includes/color.hpp"


using namespace irc;

/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Client::Client( int fd )		// main constructor
{
	_socket.pollfd.fd = fd;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	_socket_opened = true;
	_registration = NONE_SET;
	// _socket.addr = addr;	
}

Client::Client( string nick )  // WARNING: TESTING PURPOSE constructor
{
	_nickname = nick;
	_socket.pollfd.fd = 0;
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	_socket_opened = true;
	_registration = NONE_SET;	
}

Client::Client( const Client& rhs ) // copy constructor
{
	*this = rhs;
}

Client&	Client::operator=( const Client& rhs ) // copy operator overload
{
	set_nickname(rhs.get_nickname());
	set_username(rhs.get_username());
	set_hostname(rhs.get_hostname());
	
	_socket.pollfd.fd = rhs.get_fd();
	_socket.pollfd.events = POLLIN | POLLOUT | POLLERR;
	_socket_opened = rhs.is_opened();
	
	_registration = rhs.get_registration_flags();
	// _socket.addr = rhs.get_addr_copy();
	return *this;
}

Client::~Client( void ) // destructor
{
	// freeaddrinfo( reinterpret_cast<struct addrinfo*>(&get_addr()) );
}


/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

bool	Client::operator==( const Client& rhs) const
{
	return this->get_nickname() == rhs.get_nickname();
}


/*-----------------------GETTERS----------------------*/

const string&	Client::get_nickname( void ) const { return _nickname; }
const string&	Client::get_username( void ) const { return _username; }
string			Client::get_hostname( void ) const {return _hostname; } 
const string&	Client::get_realname( void ) const { return _realname; }

string Client::get_prefix( void ) 
{ 
	return (":" + get_nickname() + "!" + get_username() + "@" + get_hostname() + " ");
}


int				Client::get_registration_flags( void ) const { return _registration; }

// where is t_pollfd*	Client::get_pollfd_array( void ) ???
t_pollfd&		Client::get_pollfd( void ) { return _socket.pollfd; }
//t_addr6&		Client::get_addr6_ref( void ) { return _socket.addr6; }
//t_addr6			Client::get_addr6_copy( void ) const { return _socket.addr6; }

const int&		Client::get_fd( void ) const { return _socket.pollfd.fd; }
short			Client::get_events( void ) const { return (_socket.pollfd.events); }
short			Client::get_revents( void ) const { return (_socket.pollfd.revents); }

string&			Client::get_buff( u_int buff_i )
{ 
	if (buff_i == BUFFIN)
		return _buff[0];
	if (buff_i == BUFFOUT)
		return _buff[1];
	return _buff[0];
}

const string&	Client::get_source( void ) const { return _nickname; }


/*-----------------------SETTERS----------------------*/

void	Client::set_nickname( const string& nickname ) { _nickname = nickname; }
void	Client::set_username( const string& username ) { _username = username; }
void	Client::set_hostname( const string& hostname ) { _hostname = hostname; }
void	Client::set_realname( const string& realname ) { _realname = realname; }

void	Client::set_registration_flags( const e_registration& flag ) 
{
	this->_registration |= flag;
}


/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

void	Client::append_buff( u_int buff_i, const string& content )
{
	if (buff_i == BUFFIN)
		_buff[0].append(content);
	if (buff_i == BUFFOUT)
		_buff[1].append(content);
}

void	Client::clear_buff( u_int buff_i )
{
	if (buff_i == BUFFIN)
		_buff[0].clear();
	if (buff_i == BUFFOUT)
		_buff[1].clear();
}

void	Client::trim_buff( u_int buff_i, size_t len )
{
	string	*buff = NULL;

	if (buff_i == BUFFIN)
		buff = &buff[0];
	if (buff_i == BUFFOUT)
		buff = &buff[1];
	if (!buff)
		return ;
	*buff = buff->substr(0, len);
}

bool	Client::is_event( int event ) const { return (get_revents() & event); }
bool	Client::is_opened( void ) const { return (_socket_opened); }
bool	Client::is_nickname_set( void ) const { return (_registration & NICK_SET ? true : false); } 
bool	Client::is_username_set( void ) const { return (_registration & USER_SET ? true : false); } 
bool	Client::is_password_validated( void ) const { return (_registration & PASS_SET ? true : false); } 
bool	Client::is_registered( void ) const { return (_registration & COMPLETE ? true : false); } 
// or { return  _registration == COMPLETE; }


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream&	irc::operator<<( std::ostream& o, const Client & obj )
{
	return o << obj.get_nickname();
}

