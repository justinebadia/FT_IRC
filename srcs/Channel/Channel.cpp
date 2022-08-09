/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:34:51 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/09 13:32:08 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp" // includes <iostream><list><map><string>

using namespace irc;
using std::string;

Channel::Channel( void ) : _password_required(false), _password(string()) {} // default constructor [PRIVATE]

Channel::Channel( const Channel& other ) : _password_required(false), _password(string()) 
{ 
	*this = other; 
} // copy constructor [PRIVATE]

Channel& Channel::operator=( const Channel& rhs )	// copy operator overload
{
	_name = rhs.get_name();
	_owner = rhs.get_owner();
	_invite_only = rhs.get_is_invite_only();
	_password_required = rhs.get_is_password_required();
	// _password = rhs.get_password();

	return *this;
}							


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Channel::Channel( const string& channel_name, Client* channel_owner )			// no password constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _invite_only(false)
	, _password_required(false)
	, _password(string())
{
	add(channel_owner);
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // password required constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _invite_only(false)
	, _password_required(true)
	, _password(channel_password)
{
}

Channel::~Channel( void ) // destructor
{

}


/*-----------------------GETTERS----------------------*/

const string&		Channel::get_name( void ) const { return _name; }

Client*				Channel::get_owner( void ) const { return _owner; } 

bool				Channel::get_is_invite_only( void ) const { return _invite_only; } 

bool				Channel::get_is_password_required( void ) const { return _password_required; } 

string&				Channel::get_password( void ) { return _password; }

t_client_ptr_list	Channel::get_banlist( void ) { return _banlist; }

// Channel::e_permission		Channel::get_permission( Client* client )
// {
// 	iterator it = _banlist.begin();
// 	iterator ite = _banlist.end();

// 	for (; it != ite; it++)
// 	{
// 		if ((*it).first == client)					// pair's first == Client*	
// 			return (*it).second;					// pair's second ==  e_permission
// 	}
// 	return (e_permission)FAIL;						// Didn't find the Client in the channel_banlist
// }



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

// int		Channel::set_permission( Client* client, e_permission type )
// {	
// 	iterator it = _banlist.begin();
// 	iterator ite = _banlist.end();

// 	for (; it != ite; it++)
// 	{
// 		if ((*it).first == client)					// pair's first == Client*
// 			(*it).second = type;					// pair's second == e_permission
// 		return SUCCESS;
// 	}
// 	return FAIL;
// }

/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

void	Channel::join_public( Client* client )
{
	if (this->_invite_only == true)
	{
		std::cout << "invite only" << std::endl;
		return ;
	}
	this->add(client);
}

void	Channel::join_private( Client* client, const string& password )
{
	if (this->_invite_only == true)
	{
		std::cout << "invite only" << std::endl;
		return ;
	}
	
	if (this->_password != password)
	{
		std::cout << "wrong password" << std::endl;
		return ;
	}
	this->add(client);
}

void	Channel::add( Client* client )
{
	if (this->is_banned(client) == false)
		_banlist.push_back(client);
}

// void	Channel::kick( Client* source, Client* target )
// {
// 	if ( this->is_operator(source) == true )
// 	{
// 		if (this->get_permission(source) < this->get_permission(target))
// 		{
// 			std::cout << "don't have the permission to kick" << std::endl;
// 	 		return ;
// 		}
// 	}
// }

// void	Channel::ban( Client* source, Client* target )
// {
// 	if ( this->is_operator(source) == true )
// 	{
// 		if (this->get_permission(source) < this->get_permission(target))
// 		{
// 			std::cout << "don't have the permission to ban" << std::endl;
// 	 		return ;
// 		}
// 	}
// }

bool	Channel::is_operator( Client* member )
{
	if (member == this->get_owner())
		return true;
	return false;
}

bool	Channel::is_banned( Client* member )
{
	iterator it = _banlist.begin();
	iterator ite = _banlist.end();
	
	for (; it != ite; it++)
	{
		if ((*it) == member)
		{
			std::cout << "member already in this channel_banlist" << std::endl;
			return true ;
		}
	}
	return false;
}

// bool	Channel::is_empty( void )
// {
// 	if (_banlist.size() == 0)
// 		return true;
// }


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	return o; 
}

