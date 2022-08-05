/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/04 16:29:54 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <list>
#include <map>
#include <iostream>

#include "Client.hpp"
#include "Channel.hpp"
#include "typedef.hpp"

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Database {

private:

	/*---------------PROHIBITED CONSTRUCTOR--------------*/
	Database( const Database& other );						// copy constructor
	Database& operator=( const Database& other );			// copy operator overload 
	Database( void );	// main constructor
	~Database( void );	// default destructor
	
	/*--------------------ATTRIBUTES---------------------*/

	t_client_list			_client_list;
	t_channel_list			_channel_list;
	
	t_channel_clients_map	_channel_clients_list; // TO DO

public:
	/*-----------------------GETTERS----------------------*/
	
	static Database&		get_Database(  ); // singleton

	// [Client related getters]
	const t_client_list&	get_client_list( void );
	size_t					get_client_count( void );
	Client*					get_client( const int& fd );
	Client*					get_client( const string& nickname );
	t_client_ptr_list		get_clients_in_channel( const string& chan_name );
	Channel*				get_channel( const string& chan_name );
	// Channel*				get_channel( const Client* client );	

	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/
	
	void		init_Database( void );
	void		add_client( const Client& client );
	void		remove_client( const int& fd );
	void		remove_client( const string& nickname );
	void		remove_channel( const string& chan_name );
	void		remove_client_from_channel( const string& nickname, const string& chan_name );
	void		remove_all_clients_from_channel( const string& chan_name );

	
};



} // namespace irc end bracket

#endif
