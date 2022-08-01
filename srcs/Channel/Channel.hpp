#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <string>

namespace irc {

using std::string;
using std::list;


class Client;

class Channel {

private:
	Channel( void ); // default constructor
	Channel( const Channel& other ); // copy constructor
	Channel& operator=( const Channel& other ); // copy operator overload

	const string	_channel_name;
	Client*		_channel_owner;
	const bool	_password_required;
	const string	_channel_password;
	list<Client *>  _channel_clientlist;
	list<string>	_banlist; // of nicknames;

	static	int	_nb_of_channels;
	int		_nb_of_operators;

public:
	Channel( const string& channel_name, Client* channel_owner ); // public-channel constructor
	Channel( const string& channel_name, Client* channel_owner, const string& channel_password ); // private-channel constructor
	~Channel( void ); // destructor

	const string&	get_channel_name( void ) const;
	Client*		get_channel_owner( void ) const;
	const bool	get_is_password_required( void ) const;
	const string&	get_channel_password( void ) const;
	list<Client *>	get_channel_clientlist( void );
	list<string>	get_channel_banlist( void );
	static int	get_nb_of_channels( void );
	int		get_nb_of_operators( void ) const;
};

std::ostream&	operator<<( std::ostream& o, const Channel& obj );

} // namespace irc end scope

#endif
