/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/07 18:25:31 by fousse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MESSAGE_MANAGER_HPP
#define MESSAGE_MANAGER_HPP

#include "Message.hpp"
#include "Client.hpp"
#include "Database.hpp"
#include "irc_define.hpp"
#include "typedef.hpp"

#define MSG_DELIMITER	" "

using std::string;

/*============================================================================*/
namespace irc {

class Message;

class MessageManager {

private:
	

	/*--------------PROHIBITED-CONSTRUCTORS--------------*/
	MessageManager( const MessageManager& rhs ) {  };
	MessageManager& operator=( const MessageManager& rhs ) {  };	
	
	/*--------------------ATTRIBUTES---------------------*/
	Database*		_database;
	t_command_map	_command_map;
	t_reply_map		_reply_map;

	void	_init_command_map( void );
	void	_init_reply_map( void );
	
public:

	MessageManager( void );	
	MessageManager( Database* database );	// main constructor
	~MessageManager( void );			// destructor
	

	/*-----------------------GETTERS----------------------*/
	t_cmd_function_ptr		get_command_ptr( string name );
	t_reply_function_ptr	get_reply_ptr( int code );

	/*-----------------------SETTERS----------------------*/
	// void	set_server( Server* server );
	void	set_database( Database* database );


};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/
	

} // namespace irc end bracket

#endif
