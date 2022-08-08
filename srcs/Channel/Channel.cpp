/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:34:51 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/07 21:58:20 by fousse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp" // includes <iostream><list><string>

using namespace irc;
using std::string;

Channel::Channel( void ) : _password_required(false), _channel_password(string()) {} // default constructor [PRIVATE]

Channel::Channel( const Channel& other ) : _password_required(false), _channel_password(string()) 
{ 
	*this = other; 
} // copy constructor [PRIVATE]

Channel& Channel::operator=( const Channel& other ){} // copy operator overload [PRIVATE]

Channel::Channel( const string& channel_name, Client* channel_owner ) // public-channel (no password) constructor
	: _channel_name(channel_name)
	, _channel_owner(channel_owner)
	, _password_required(false)
	, _channel_password(NULL)
	, _nb_of_operators(1)

{
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // private-channel (PASSWORD) constructor
	: _channel_name(channel_name)
	, _channel_owner(channel_owner)
	, _password_required(true)
	, _channel_password(channel_password)
	, _nb_of_operators(1)
{
}

Channel::~Channel( void ) // destructor
{
}


/*-----------------------GETTERS----------------------*/
const string&	Channel::get_channel_name( void ) const { return _channel_name; }
Client*			Channel::get_channel_owner( void ) const { return _channel_owner; } 
const bool		Channel::get_is_password_required( void ) const { return _password_required; }
const string&	Channel::get_channel_password( void ) const { return _channel_password; }

/*-----------------------SETTERS----------------------*/


/*---------------OTHER-MEMBER-FUNCTIONS---------------*/


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	return o; 
}

