/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 08:24:20 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/02 08:42:51 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <string>

class Client; 

using std::string;
using std::list;

/*============================================================================*/ 
namespace irc {

class Channel {

private:

	Channel( void ); 							// default constructor
	Channel( const Channel& other );			// copy constructor
	Channel& operator=( const Channel& other ); // copy operator overload

	/*--------------------ATTRIBUTES---------------------*/
	
	const string	_channel_name;
	Client*			_channel_owner;
	const bool		_password_required;
	const string	_channel_password;
	list<Client*>	_channel_clientlist;
	list<string>	_banlist; 					// banlist of nicknames;

	static	int		_nb_of_channels;
	int				_nb_of_operators;


public:

	Channel( const string& channel_name, Client* channel_owner ); 								 	// public-channel (no password) constructor
	Channel( const string& channel_name, Client* channel_owner, const string& channel_password );	// private-channel (PASSWORD) constructor
	~Channel( void );																				// destructor


	/*-----------------------GETTERS----------------------*/

	const string&	get_channel_name( void ) const;
	Client*			get_channel_owner( void ) const;
	const bool		get_is_password_required( void ) const;
	const string&	get_channel_password( void ) const;
	list<Client*>	get_channel_clientlist( void );
	list<string>	get_channel_banlist( void );
	static int		get_nb_of_channels( void ) const;
	int				get_nb_of_operators( void ) const;

	/*-----------------------SETTERS----------------------*/




	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/




};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );


} // namespace irc end bracket

#endif

