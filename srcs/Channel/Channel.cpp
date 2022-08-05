/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:34:51 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/05 18:09:28 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp" // includes <iostream><list><string>

using namespace irc;

Channel::Channel( void ){}														// default constructor [PRIVATE]

Channel::Channel( const Channel& other ){ *this = other; }						// copy constructor [PRIVATE]

Channel& Channel::operator=( const Channel& other ){}							// copy operator overload [PRIVATE]

Channel::Channel( const string& channel_name, Client* channel_owner )			// public-channel (no password) constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _invite_only(false)
	, _password_required(false)
	, _channel_password(NULL)
{
	memberlist
	Channel::_nb_of_channels++;
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // private-channel (PASSWORD) constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _invite_only(false)
	, _password_required(true)
	, _password(channel_password)
{
	Channel::_nb_of_channels++;
}

Channel::~Channel( void ) // destructor
{
	Channel::_nb_of_channels--;
}


/*-----------------------GETTERS----------------------*/

const string&	Channel::get_name( void ) const { return _name; }

Client*			Channel::get_owner( void ) const { return _owner; } 

bool			Channel::get_is_invite_only( void ) const { return _invite_only; }

bool			Channel::get_is_password_required( void ) const { return _password_required; }

const string&	Channel::get_channel_password( void ) const { return _password; }

list<Client*, e_permission>	Channel::get_memberlist( void ) { return _memberlist; }

e_permission	Channel::get_permission( Client* client )
{
	list<Client*, e_permission>::iterator it = _memberlist.begin();
	list<Client*, e_permission>::iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first == client)
			return (*it).second;					// list< pair <Client*, e_permission> >
	}
	
	// std<<cout << "Client* not found in the _memberlist" << std::endl;
	return FAIL;
}


static int	Channel::get_nb_of_channels( void ) { return _nb_of_channels; }



/*-----------------------SETTERS----------------------*/

void	Channel::set_invite_only( bool setting ) 
{
	_invite_only = setting;
}

void	Channel::set_password( const string& password )
{
		_password_required = true;
		_password = password;
}

int		Channel::set_permission( Client* client, e_permission type )
{	
	list<Client*, e_permission>::iterator it = _memberlist.begin();
	list<Client*, e_permission>::iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first == client)
			(*it).second = type;
		return SUCCESS;
	}
	return FAIL;
}

/*---------------OTHER-MEMBER-FUNCTIONS---------------*/


bool	Channel::is_empty( void )
{
	if (_memberlist.size() == 0)
		// call the destructor
}


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	return o; 
}

