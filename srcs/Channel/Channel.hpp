/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:24:20 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/12 19:09:48 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <map>
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
		INVITE = 1,
		REGULAR = 2,
		CHANOP = 4,
		OWNER = 8
	};
	
	typedef	std::list<std::pair<Client*, e_permission> >			channel_memberlist;
	typedef	std::list<std::pair<Client*, e_permission> >::iterator	iterator;

	enum mode_flags
	{
		FLAG_O = 1,
		FLAG_I = 2,
		FLAG_T = 4,
		FLAG_K = 8,
		FLAG_B = 16,
	};
	//typedef t_client_ptr_list::iterator	iterator;

private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	

	/*---------------------ATTRIBUTES---------------------*/
	
	string								_name;						// channel_name
	Client*								_owner;
	bool								_private;					// ±p
	bool								_secret;					// ±s
	bool								_invite_only;				// ±i
	bool								_password_required;			// ±flag pour +k
	bool								_topic_by_chanop_only;		// ±t
	string								_topic;
	string								_password;
	channel_memberlist					_memberlist;
	int									_mode_flags;
	//t_client_ptr_list					_banlist;


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
	bool								get_is_private( void ) const;
	bool								get_is_secret( void ) const;
	bool								get_is_invite_only( void ) const;
	bool								get_is_password_required( void ) const;
	bool								get_is_topic_by_chanop_only( void ) const;
	string&								get_topic( void );
	string&								get_password( void );
	channel_memberlist					get_memberlist( void );
	int 								get_mode_flags( void );
	e_permission						get_permission( Client* client );


	/*-----------------------SETTERS----------------------*/
	
	int		set_permission( Client* client, e_permission type );
	void	set_mode_private( bool setting );
	void	set_mode_secret( bool setting );
	void	set_mode_invite_only( bool setting );
	void	set_mode_key_password_required( bool setting );
	void	set_mode_topic_by_chanop_only( bool setting );

	void	set_topic( const string& topic );
	void	set_password( const string& password );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	bool	is_member( Client* client );
	bool	is_owner( Client* client );
	bool	is_chanop( Client* client );
	bool	is_banned( Client* client );
	bool	is_only_banned_member_left( void );
	bool	is_empty( void );

	void	add_member( Client* client, e_permission type );
	int		remove_member( Client* client );
	void	empty_memberlist( void );

	void	transfer_ownership( void );
	int		parse_modes( string message );

};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );
	
	static int							get_nb_of_channels(  void );


} // namespace irc end bracket

#endif

