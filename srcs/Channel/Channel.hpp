/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:24:20 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/12 14:29:11 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
// #include <list>
// #include <map>
#include <string>
#include "Client.hpp"
#include "typedef.hpp"

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Channel {

public:

	/*--------------------TYPEDEF-&-ENUM-------------------*/
	
	enum e_permission									// Usage-> Channel::OWNER, not sure
	{
		BAN = 0,
		REGULAR = 1,
		CHANOP = 2,
		OWNER = 4
	};
	typedef t_client_ptr_list::iterator	iterator;

private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	

	/*---------------------ATTRIBUTES---------------------*/
	
	string								_name;						// channel_name
	Client*								_owner;
	bool								_invite_only;
	bool								_password_required;
	string								_password;
	string 								_topic;
	t_client_ptr_list					_banlist;


public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/
	
	Channel( void ); 												// default constructor
	Channel( const string& channel_name, Client* channel_owner ); 									// no password constructor
	Channel( const string& channel_name, Client* channel_owner, const string& channel_password );	// password required constructor
	Channel( const Channel& rhs );									// copy constructor
	~Channel( void );												// destructor
	Channel& operator=( const Channel& rhs );	 					// copy operator overload


	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Channel& rhs) const;


	/*-----------------------GETTERS----------------------*/
	
	const string&						get_name( void ) const;
	Client*								get_owner( void ) const;
	bool								get_is_invite_only( void ) const;
	bool								get_is_password_required( void ) const;
	string&								get_password( void );
	t_client_ptr_list					get_banlist( void );
	e_permission						get_permission( Client* client );
	string&								get_topic( void );



	/*-----------------------SETTERS----------------------*/
	
	void	set_invite_only( bool setting );
	void	set_password( const string& password );
	int		set_permission( Client* client, e_permission type );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void	join_public( Client* client );	// Warning: test, maybe everything will be done in Database
	void	join_private( Client* client, const string& password );
	void	add( Client* client );
	// void	kick( Client* source, Client* target );
	// void	ban( Client* source, Client* target );

	bool	is_operator( Client* member );
	bool	is_banned( Client* client );

};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );

} // namespace irc end bracket

#endif

