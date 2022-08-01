#include "Channel.hpp"

namespace irc 
{

Channel::Channel( void ){} // default constructor [PRIVATE]

Channel::Channel( const Channel& other ){ *this = other; } // copy constructor [PRIVATE]

Channel& Channel::operator=( const Channel& other ){} // copy operator overload [PRIVATE]

Channel::Channel( const string& channel_name, Client* channel_owner ) // public-channel constructor
	: _channel_name(channel_name)
	, _channel_owner(channel_owner)
	, _password_required(false)
	, _channel_password(NULL)
	, _nb_of_operators(1)

{
	Channel::_nb_of_channels++;
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // private-channel constructor
	: _channel_name(channel_name)
	, _channel_owner(channel_owner)
	, _password_required(true)
	, _channel_password(channel_password)
	, _nb_of_operators(1)
{
	Channel::_nb_of_channels++;
}

Channel::~Channel( void ) // destructor
{

}

const string&	Channel::get_channel_name( void ) const { return _channel_name; }
Client*		Channel::get_channel_owner( void ) const { return _channel_owner; } 
const bool	Channel::get_is_password_required( void ) const { return _password_required; }
const string&	Channel::get_channel_password( void ) const { return _channel_password; }

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	return o; 
}

}
