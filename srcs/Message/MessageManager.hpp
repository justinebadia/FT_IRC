/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/07 22:14:25 by fousse           ###   ########.fr       */
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
class Server;


class MessageManager {

private:
	friend class Server;
	/*--------------PROHIBITED-CONSTRUCTORS--------------*/
	MessageManager( const MessageManager& rhs ) {  };
	MessageManager& operator=( const MessageManager& rhs ) {  };
	MessageManager( void );	
	MessageManager( Database* database );	// main constructor
	~MessageManager( void );			// destructor
	
	/*--------------------ATTRIBUTES---------------------*/
	static Server*			_server;
	static Database*		_database;
	static t_command_map	_command_map;
	static t_reply_map		_reply_map;

	static void	_init_command_map( void );
	static void	_init_reply_map( void );
	
public:

	/*-----------------------GETTERS----------------------*/
	static	t_cmd_function_ptr		get_command_ptr( string name );
	static	t_reply_function_ptr	get_reply_ptr( int code );

	/*-----------------------SETTERS----------------------*/
	static void	set_server( Server* server );
	static void	set_database( Database* database );

	/*------------------COMMANDS-FUNCTIONS----------------*/
	static void	execute_commands( Client& client );
	static void	cmd_nick( Message& msg );
	static void	cmd_user( Message& msg );

	/*------------------REPLIES-FUNCTIONS-----------------*/
	static void run_reply( int code, Message& msg );

};

	

} // namespace irc end bracket

#endif
