/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/08 13:19:18 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

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


class CommandManager {

private:
	friend class Server;
	/*--------------PROHIBITED-CONSTRUCTORS--------------*/
	CommandManager( const CommandManager& rhs ) {  };
	CommandManager& operator=( const CommandManager& rhs ) {  };
	CommandManager( void );	
	CommandManager( Database* database );	// main constructor
	~CommandManager( void );			// destructor
	
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
	static void	cmd_whois( Message& msg );

	/*------------------REPLIES-FUNCTIONS-----------------*/
	static void run_reply( int code, Message& msg );
	static void rpl_welcome( Message& msg );
	static void rpl_whoisuser( Message& msg );
	static void rpl_whoisserver(Message& msg );
	static void rpl_whoisoperator(Message& msg );
	static void rpl_endofwhois( Message& msg );
	static void rpl_whoischannels( Message& msg );
	/*NICK REPLIES*/
	static void err_nonicknamegiven( Message& msg);
	static void err_erroneusnickname( Message& msg);
	static void err_nicknameinuse( Message& msg);
	static void err_nickcollision( Message& msg);
	/*USERS REPLIES*/
	static void err_nosuchserver( Message& msg);
	static void err_userdisabled( Message& msg );
	static void rpl_nousers( Message& msg );
	static void rpl_usersstart( Message& msg );
	static void rpl_endofusers( Message& msg );
	/*USERS MSG REPLIES*/
	static void err_needmoreparams( Message& msg );
	static void err_alreadyregistered( Message& msg );

};

	

} // namespace irc end bracket

#endif
