/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 16:01:18 by fousse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Message.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Database.hpp"
#include "irc_define.hpp"
#include "typedef.hpp"

#define MSG_DELIMITER	" "

using std::string;

/*============================================================================*/
namespace irc {

class MessageManager {

private:

	class Message;

	/*--------------PROHIBITED-CONSTRUCTORS--------------*/
	MessageManager( const MessageManager& rhs ) {  };
	MessageManager& operator=( const MessageManager& rhs ) {  };	
	
	/*--------------------ATTRIBUTES---------------------*/
	Server*			_server;
	Database*		_database;
	t_command_map	_command_map;
	t_reply_map		_reply_map;

	void	_init_command_map( void );
	void	_init_reply_map( void );
	
public:

	MessageManager( void );	
	MessageManager( Server* server );	// main constructor
	~MessageManager( void );			// destructor
	

	/*-----------------------GETTERS----------------------*/
	t_cmd_function_ptr		get_command_ptr( string name );
	t_reply_function_ptr	get_reply_ptr( int code );

	/*-----------------------SETTERS----------------------*/

};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/
	

} // namespace irc end bracket

#endif
