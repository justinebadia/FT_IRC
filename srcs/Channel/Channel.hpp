/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:24:20 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/08 13:30:43 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <map>
#include <string>
#include "Client.hpp"

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Channel {

public:

	/*--------------------TYPEDEF-&-ENUM-------------------*/
	
	static enum e_permission									// Usage-> Channel::OWNER, not sure
	{
		BAN = 0,
		REGULAR = 1,
		CHANOP = 2,
		OWNER = 4
	};

	typedef std::list<std::pair<Client*, e_permission> >			channel_memberlist;
	typedef std::list<std::pair<Client*, e_permission> >::iterator	iterator;


private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	Channel( void ); 												// default constructor
	Channel( const Channel& rhs );									// copy constructor
	Channel& operator=( const Channel& rhs );	 					// copy operator overload


	/*---------------------ATTRIBUTES---------------------*/
	
	const string						_name;						// channel_name
	Client*								_owner;
	bool								_invite_only;
	bool								_password_required;
	string								_password;
	channel_memberlist					_memberlist;


public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/
	
	Channel( const string& channel_name, Client* channel_owner ); 									// no password constructor
	Channel( const string& channel_name, Client* channel_owner, const string& channel_password );	// password required constructor
	~Channel( void );												// destructor

	
	/*-----------------------GETTERS----------------------*/
	
	const string&						get_name( void ) const;
	Client*								get_owner( void ) const;
	bool								get_is_invite_only( void ) const;
	bool								get_is_password_required( void ) const;
	string&								get_password( void );
	channel_memberlist					get_memberlist( void );
	e_permission						get_permission( Client* client );



	/*-----------------------SETTERS----------------------*/
	
	void	set_invite_only( bool setting );
	void	set_password( const string& password );
	int		set_permission( Client* client, e_permission type );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void	join_public( Client* client, e_permission type );	// Warning: test, maybe everything will be done in Database
	void	join_private( Client* client, e_permission type, const string& password );
	void	add( Client* client, e_permission type );
	void	kick( Client* source, Client* target );
	void	ban( Client* source, Client* target );

	bool	is_channel_member( Client* client );
	bool	is_empty( void );	// if the memberlist is empty, destroy the channel 


};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );
	
	static int							get_nb_of_channels(  void );


} // namespace irc end bracket

#endif

