/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <tshimoda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:34:51 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/11 18:33:15 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp" // includes <iostream><list><map><string>

using namespace irc;
using std::string;


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Channel::Channel( void ) // default constructor 
	: _name("")
	, _owner(NULL)
	, _private(false)
	, _secret(false)
	, _invite_only(false)
	, _topic_by_chanop_only(false)
	, _topic("")
	, _password("")
	, _memberlist(0) 
	{}

Channel::Channel( const Channel& rhs ) // copy constructor
{ 
	*this = rhs; 
} 

Channel& Channel::operator=( const Channel& rhs )	// copy operator overload
{
	_name  = rhs._name;
	_owner = rhs._owner;
	_private = rhs._private;
	_secret = rhs._secret;
	_invite_only = rhs._invite_only;
	_topic_by_chanop_only = rhs._topic_by_chanop_only;
	_topic = rhs._topic;
	_password = rhs._password;
	_memberlist = rhs._memberlist;

	return *this;
}							

Channel::Channel( const string& channel_name, Client* channel_owner )			// no password constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _private(false)
	, _secret(false)
	, _invite_only(false)
	, _topic_by_chanop_only(false)
	, _topic("")
	, _password("")
	, _memberlist(0) 
{
	add_member(channel_owner, OWNER);
	// DATABASE ADD THIS CHANNEL IN THE CHANNEL LIST
}

Channel::Channel( const string& channel_name, Client* channel_owner, const string& channel_password ) // password required constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _private(true)
	, _secret(false)
	, _invite_only(false)
	, _topic_by_chanop_only(true)
	, _topic("")
	, _password(channel_password)
	, _memberlist(0) 
{
	add_member(channel_owner, OWNER);
	// DATABASE ADD THIS CHANNEL IN THE CHANNEL LIST
}

Channel::~Channel( void ) // destructor
{
	empty_memberlist();
	// DATABASE MUST REMOVE THIS CHANNEL FROM LIST OF CHANNELS
}


/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

bool	Channel::operator==( const Channel& rhs) const
{
	if (get_name() == rhs.get_name())
		return true;
	return false;
}

/*-----------------------GETTERS----------------------*/

const string&		Channel::get_name( void ) const { return _name; }

Client*				Channel::get_owner( void ) const { return _owner; } 

bool				Channel::get_is_private( void ) const { return _private; }

bool				Channel::get_is_secret( void ) const { return _secret; }

bool				Channel::get_is_invite_only( void ) const { return _invite_only; } 

bool				Channel::get_is_topic_by_chanop_only( void ) const { return _topic_by_chanop_only; }

string&				Channel::get_topic( void ) { return _topic; }

string&				Channel::get_password( void ) { return _password; }

Channel::channel_memberlist	Channel::get_memberlist( void ) { return _memberlist; }

//t_client_ptr_list	Channel::get_banlist( void ) { return _banlist; }

Channel::e_permission		Channel::get_permission( Client* client )
{
 	iterator it = _memberlist.begin();
 	iterator ite = _memberlist.end();

 	for (; it != ite; it++)
 	{
 		if ((*it).first->get_nickname() == client->get_nickname())					// pair's first == Client*	
 			return (*it).second;													// pair's second ==  e_permission
 	}
 	return (e_permission)FAIL;														// Didn't find the Client in the channel_banlist
}


/*-----------------------SETTERS----------------------*/


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

void	Channel::set_mode_private( bool setting )
{
	_private = setting;
}

void	Channel::set_mode_secret( bool setting )
{
	_secret = setting;
}

void	Channel::set_mode_invite_only( bool setting ) 
{
	_invite_only = setting;
}

void	Channel::set_mode_topic_by_chanop_only( bool setting )
{
	_topic_by_chanop_only = setting;
}

void	Channel::set_topic( const string& topic )
{
	_topic = topic;
}

void	Channel::set_password( const string& password )
{
	_password = password;
}

/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

bool	Channel::is_member( Client* client )
{
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first->get_nickname() == client->get_nickname())
			return true;
	}
	return false;

}

bool	Channel::is_owner( Client* client )
{
	if (get_permission(client) == OWNER)
		return true;

	return false;
}

bool	Channel::is_chanop( Client* client )
{
	if (get_permission(client) == CHANOP)
		return true;
	
	return false;
}

bool	Channel::is_banned( Client* client )
{
	if (get_permission(client) == BAN )
		return true;

	return false;
}

bool	Channel::is_only_banned_member_left( void )
{
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).second != BAN)
		return false;
	}
	return true;
}


bool	Channel::is_empty( void )
{
	if (_memberlist.size() == 0)
		return true;

	return false;
}

void	Channel::add_member( Client* client, e_permission type )
{
	if (is_member(client) == false)
		_memberlist.push_back(std::make_pair(client, type));
}

int	Channel::remove_member( Client* client )
{
	iterator it = _memberlist.begin();
	iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if ((*it).first->get_nickname() == client->get_nickname())
		{
			if (is_owner(client))
			{
				if (_memberlist.size() == 1)
				{
					empty_memberlist();
					// RETURN INT TO DATABASE TO REMOVE THE CHANNEL
					return 22; // WARNING
				}
				else
				{
					_memberlist.erase(it);
					//transfer_ownership(); WARNING INCORECCT!!!!!

					if (is_only_banned_member_left() == true)
					{
						// retourne un int pour dire a la database de detruire ce channel
						return 1;
					}
					else
						break;
				}
			}
			_memberlist.erase(it);
			break;
		}
	}
	return 42; // WARNING
}

void	Channel::empty_memberlist( void )
{
	_memberlist.clear();
}

void	Channel::transfer_ownership( void )
{
	Channel::iterator it = _memberlist.begin();
	Channel::iterator ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if (get_permission((*it).first) == CHANOP)
		{
			(*it).second = OWNER;
			return ;
		}
	
	}
	it = _memberlist.begin();
	ite = _memberlist.end();

	for (; it != ite; it++)
	{
		if (get_permission((*it).first) == REGULAR)
		{
			(*it).second = OWNER;
			return ;
		}
	}

	// ELSE IL NE RESTE QUE DU MONDE BAN DANS LA MEMBERLIST, IL FAUT ENVOYER UN INT POUR REMOVE LE CHANNEL DE LA DATABASE
	// OU FAIRE UNE FONCTION IS ONLY BANNED_MEMBER_LEFT
	
}






/*
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
*/

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


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	return o; 
}

