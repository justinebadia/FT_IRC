/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:34:51 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/06 12:14:53 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp" // includes <iostream><list><map><string>

using namespace irc;

/*---------------PROHIBITED-CONSTRUCTORS--------------*/

Channel::Channel( void ){}														// default constructor [PRIVATE]

Channel::Channel( const Channel& other ){ *this = other; }						// copy constructor [PRIVATE]

Channel& Channel::operator=( const Channel& other ){}							// copy operator overload [PRIVATE]


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Channel::Channel( const string& channel_name, Client* channel_owner )			// no password constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _invite_only(false)
	, _password_required(false)
	, _channel_password(NULL)
{
	add_member(channel_owner, OWNER);

	Channel::_nb_of_channels++;
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // password required constructor
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

const string&		Channel::get_name( void ) const { return _name; }

Client*				Channel::get_owner( void ) const { return _owner; } 

bool				Channel::get_is_invite_only( void ) const { return _invite_only; }

bool				Channel::get_is_password_required( void ) const { return _password_required; }

const string&		Channel::get_channel_password( void ) const { return _password; }

channel_memberlist	Channel::get_memberlist( void ) { return _memberlist; }

e_permission		Channel::get_permission( Client* client )
{
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first == client)					// pair's first == Client*	
			return (*it).second;					// pair's second ==  e_permission
	}
	return (e_permission)FAIL;						// Didn't find the Client in the channel_memberlist
}



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
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first == client)					// pair's first == Client*
			(*it).second = type;					// pair's second == e_permission
		return SUCCESS;
	}
	return FAIL;
}

/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

void	Channel::join_public( Client* client, e_permission type )
{
	if (this->invite_only == true)
	{
		std::cout << "invite only" << std::endl;
		return ;
	}
	this->add(client, type);
}

void	Channel::join_private( Client* client, e_permission type, const string& password )
{
	if (this->invite_only == true)
	{
		std::cout << "invite only" << std::endl;
		return ;
	}
	
	if (this->password != password)
	{
		std::cout << "wrong password" << std::endl;
		return ;
	}
	this->add(client, type);
}

void	Channel::add( Client* client, e_permission type )
{
	if (this->is_channel_member(client) == false)
		_memberlist.push_back(client, type);
}

void	Channel::kick( Client* source, Client* target )
{
	if (this->is_channel_member(source) == true && this->is_channel_member(target) == true)
	{
		if (this->get_permisison(source) < this->get_permission(target)
		{
			std::cout << "don't have the permission to kick" << std::endl;
	 		return ;
		}
	}
}

void	Channel::ban( Client* source, Client* target )
{
	if (this->is_channel_member(source) == true && this->is_channel_member(target) == true)
	{
		if (this->get_permisison(source) < this->get_permission(target)
		{
			std::cout << "don't have the permission to ban" << std::endl;
	 		return ;
		}
	}
}

bool	Channel::is_channel_member( Client* member )
{
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();
	
	for (; it != ite; it++)
	{
		if ((*it).first == member)
		{
			std::cout << "member already in this channel_memberlist" << std::endl;
			return true ;
		}
	}
	return false;
}

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

int	Channel::get_nb_of_channels( void ) 
{ 
	return Channel::_nb_of_channels; 
}

int Channel::_nb_of_channels = 0;	// initialization of the static int

