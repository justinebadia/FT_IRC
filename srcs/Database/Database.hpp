/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <tshimoda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/20 15:04:16 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <iostream>

#include "Client.hpp"
#include "Channel.hpp"
#include "typedef.hpp"

/*============================================================================*/ 
namespace irc {

class Database {

private:

	/*---------------PROHIBITED CONSTRUCTOR--------------*/

	Database( const Database& other );						// copy constructor
	Database& operator=( const Database& other );			// copy operator overload 
	

	/*--------------------ATTRIBUTES---------------------*/

	t_client_list			_client_list;
	t_channel_list			_channel_list;
	t_invite_coupon_list	_invite_coupon_list;


public:
	
	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

	// static Database&		get_Database(  ); 				// singleton
	Database( void );										// main constructor
	~Database( void );										// default destructor


	/*-----------------------GETTERS----------------------*/ 

	const t_client_list&	get_client_list( void );
	const t_client_ptr_list	get_client_ptr_list( void );
	size_t					get_client_count( void );
	Client*					get_client( const int& fd );
	Client*					get_client( const string& nickname );
	t_client_ptr_list		get_clients_in_channel( const string& chan_name );
	t_client_ptr_list		get_clients_in_channel( Channel* channel );

	Channel*				get_channel( const string& chan_name );
	Channel*				get_channel( const Client* client );
	size_t					get_channel_count( void );
	t_channel_list			get_channel_list( void );
	t_channel_ptr_list		get_channel_list_of_client( Client* client );
	
	t_client_ptr_list		get_channel_in_common_recipient_list( Client* client );
	t_invite_coupon_list	get_invite_coupon_list( void );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/
	
	void		init_Database( void );
	int			add_client_list( const Client& client );
	int			add_channel_list( const Channel& channel );
															
	bool		is_client_listed( const Client& client );
	bool		is_client_listed( const int& fd );
	bool		is_client_listed( const string& nickname );
	bool		is_channel_listed( const Channel& channel );
	bool		is_channel_listed( const string& chan_name );
	bool		is_channel_empty( Channel* channel );

	void		delete_client_from_all_lists( Client* client );
	void		delete_inactive_channels( void );

	void		print_client_list( void );
	void		print_channel_list( void );
	void		print_invite_coupon_list( void );

	void		create_invite_coupon( Client* client, Channel* channel );
	int			use_invite_coupon( Client* client, Channel* channel );

	void		clean_database( void );

};


} // namespace irc end bracket

#endif
