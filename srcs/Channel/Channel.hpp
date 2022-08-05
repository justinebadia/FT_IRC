/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:24:20 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/05 17:57:22 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <map>
#include <string>

class Client; 

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Channel {


enum e_permission													// Warning: test
{
	OWNER = 1,
	OPERATOR = 2,
	REGULAR = 4,
	BAN = 8
}

private:

	Channel( void ); 												// default constructor
	Channel( const Channel& rhs );									// copy constructor
	Channel& operator=( const Channel& rhs );	 					// copy operator overload

	/*--------------------ATTRIBUTES---------------------*/
	
	const string						_name;						// _name for channel name
	Client*								_owner;
	bool								_invite_only;
	bool								_password_required;
	const string						_password;
	list<pair<Client*, e_permission>	_memberlist;				// Warning: test

	static	int							_nb_of_channels;


public:

	Channel( const string& channel_name, Client* channel_owner ); 	// no password constructor
	Channel( const string& channel_name, Client* channel_owner, const string& channel_password ); // password required constructor
	~Channel( void );												// destructor


	/*-----------------------GETTERS----------------------*/

	const string&						get_name( void ) const;
	Client*								get_owner( void ) const;
	bool								get_is_invite_only( void ) const;
	bool								get_is_password_required( void ) const;
	string&								get_password( void ) const;
	list<pair<Client*, e_permission> >	get_memberlist( void );
	e_permission						get_permission( Client* client );

	static int							get_nb_of_channels( void );

	/*-----------------------SETTERS----------------------*/

	void	set_invite_only( bool setting );
	void	set_password( const string& password );
	int		set_permission( Client* client, e_permission type );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	int		add_member( Client* client );
	int		remove_member( Client* client );
	int		promote_member( Client* client );
	int		demote_member( Client* client );
	int		ban_member( Client* client );

	bool	is_empty( void );	// if the memberlist is empty, destroy the channel 

};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );


} // namespace irc end bracket

#endif

