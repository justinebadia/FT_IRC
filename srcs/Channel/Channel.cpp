

#include "Channel.hpp" // includes <iostream><list><map><string>
#include "Message.hpp"
#include "typedef.hpp"

#include <iostream>
// #include <list>
// #include <map>
#include <string>

#include "Client.hpp"
#include "typedef.hpp"

using namespace irc;
using std::string;


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Channel::Channel( void ) // default constructor 
	: _name("")
	, _owner(NULL)
	, _private(false)
	, _secret(false)
	, _invite_only(false)
	, _password_required(false)
	, _topic_by_chanop_only(false)
	, _topic("")
	, _password("")
	, _memberlist(0) 
	, _mode_flags(0)
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
	_password_required = rhs._password_required;
	_topic_by_chanop_only = rhs._topic_by_chanop_only;
	_topic = rhs._topic;
	_password = rhs._password;
	_memberlist = t_channel_memberlist(rhs._memberlist);	// WARNING algo de copy???
	_mode_flags = rhs._mode_flags;

	return *this;
}							

Channel::Channel( const string& channel_name, Client* channel_owner )			// no password constructor
	: _name(channel_name)
	, _owner(channel_owner)
	, _private(false)
	, _secret(false)
	, _invite_only(false)
	, _password_required(false)
	, _topic_by_chanop_only(false)
	, _topic("")
	, _password("")
	, _memberlist(0) 
	, _mode_flags(0)
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
	, _mode_flags(0)
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

bool				Channel::get_is_password_required( void ) const { return _password_required; } 

bool				Channel::get_is_topic_by_chanop_only( void ) const { return _topic_by_chanop_only; }

string&				Channel::get_topic( void ) { return _topic; }

string&				Channel::get_password( void ) { return _password; }

t_channel_memberlist	Channel::get_memberlist( void ) { return _memberlist; }

int 				Channel::get_mode_flags( void ) { return _mode_flags; }

//t_client_ptr_list	Channel::get_banlist( void ) { return _banlist; }

e_permission		Channel::get_permission( Client* client )
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

t_client_ptr_list	Channel::get_clients_matching_permissions( int type )
{
	t_client_ptr_list	client_list;

	iterator it = _memberlist.begin();
 	iterator ite = _memberlist.end();

	for(; it != ite; it++)
	{
		if ((*it).second == type )
		{
			client_list.push_back((*it).first);
		}
	}
	return (client_list);
}

t_client_ptr_list	Channel::get_clients_not_matching_permissions( int type )
{
	t_client_ptr_list	client_list;

	iterator it = _memberlist.begin();
 	iterator ite = _memberlist.end();

	for(; it != ite; it++)
	{
		if ((*it).second != type )
		{
			client_list.push_back((*it).first);
		}
	}
	return (client_list);
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

void	Channel::set_mode_key_password_required( bool setting )
{
	_password_required = setting;
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
			std::cout << "BEFORE ERASE\n";
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

// void	Channel::transfer_ownership( void )
// {
// 	Channel::iterator it = _memberlist.begin();
// 	Channel::iterator ite = _memberlist.end();

// 	for (; it != ite; it++)
// 	{
// 		if (get_permission((*it).first) == CHANOP)
// 		{
// 			(*it).second = OWNER;
// 			return ;
// 		}
	
// 	}
// 	it = _memberlist.begin();
// 	ite = _memberlist.end();

// 	for (; it != ite; it++)
// 	{
// 		if (get_permission((*it).first) == REGULAR)
// 		{
// 			(*it).second = OWNER;
// 			return ;
// 		}
// 	}

// 	// ELSE IL NE RESTE QUE DU MONDE BAN DANS LA MEMBERLIST, IL FAUT ENVOYER UN INT POUR REMOVE LE CHANNEL DE LA DATABASE
// 	// OU FAIRE UNE FONCTION IS ONLY BANNED_MEMBER_LEFT
	
// }






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


int Channel::parse_modes( string message )
{
	if (message.at(0) == '+')
	{
		for (int i = 1; i != message.length(); i++)
		{
			if (message[i] == 'o')
				_mode_flags |= mode_flags::FLAG_O;
			else if (message[i] == 'i')
			{
				set_mode_invite_only(true);
				_mode_flags |= mode_flags::FLAG_I;
			}
			else if (message[i] == 't')
			{
				set_mode_topic_by_chanop_only(true);
				_mode_flags |= mode_flags::FLAG_T;
			}
			else if (message[i] == 'k')
			{
				set_mode_key_password_required(true);
				_mode_flags |= mode_flags::FLAG_K;
			}
			else if (message[i] == 'b')
				_mode_flags |= mode_flags::FLAG_B;
		}
	}
	else if (message.at(0) == '-')
	{
		for (int i = 1; i != message.length(); i++)
		{
			if (message[i] == 'o')
				_mode_flags &= mode_flags::FLAG_O;
			else if (message[i] == 'i')
			{
				set_mode_invite_only(false);
				_mode_flags &= mode_flags::FLAG_I;
			}
			else if (message[i] == 't')
			{
				set_mode_topic_by_chanop_only(false);
				_mode_flags &= mode_flags::FLAG_T;
			}
			else if (message[i] == 'k')
			{
				set_mode_key_password_required(false);
				_mode_flags &= mode_flags::FLAG_K;
			}
			else if (message[i] == 'b')
				_mode_flags &= mode_flags::FLAG_B;
		}
	}
	return _mode_flags;
}


/*----------------NON-MEMBER-FUNCTIONS----------------*/

std::ostream& operator<<( std::ostream& o, const Channel& obj ) 
{ 
	o << "Channel name: " << obj.get_name() << std::endl;
	return o; 
}

