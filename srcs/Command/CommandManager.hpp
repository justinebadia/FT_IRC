/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <tshimoda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/13 13:18:11 by tshimoda         ###   ########.fr       */
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
	CommandManager( const CommandManager& ) {  };
	CommandManager& operator=( const CommandManager& ) { return *this; };
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

	static bool	_is_unregistered_allowed( const string& cmd_name );
	
public:

	/*-----------------------GETTERS----------------------*/
	static	t_cmd_function_ptr		get_command_ptr( string name );
	static	t_reply_function_ptr	get_reply_ptr( int code );

	/*-----------------------SETTERS----------------------*/
	static void	set_server( Server* server );
	static void	set_database( Database* database );

	/*------------------COMMANDS-FUNCTIONS----------------*/
	static void	execute_commands( Client& client );
	static void	execute_commands_registration( Client& client );

	// COMMANDS []WARNING[] TEMPORARILY IN ALPHABETICAL ORDER
	static void cmd_invite( Message& msg );
	static void cmd_join( Message& msg );
	static void	cmd_kick( Message& msg );
	static void cmd_mode( Message& msg );
	static void	cmd_nick( Message& msg );
	static void	cmd_oper( Message& msg );
	static void	cmd_pass( Message& msg );
	static void cmd_ping( Message& msg );
	static void cmd_privmsg( Message& msg );
	static void cmd_quit( Message& msg );
	static void	cmd_topic( Message& msg );
	static void	cmd_user( Message& msg );
	static void	cmd_whois( Message& msg );

	/*------------------REPLIES-FUNCTIONS-----------------*/
	static void run_reply( int code, Message& msg );

	// NUMERIC REPLIES
	static void rpl_welcome( Message& msg );			//[001] ????
	static void rpl_whoisuser( Message& msg );			//[311] WHOIS
	static void rpl_whoisserver(Message& msg );			//[312] WHOIS
	static void rpl_whoisoperator(Message& msg );		//[313] WHOIS
	static void rpl_endofwhois( Message& msg );			//[318] WHOIS
	static void rpl_whoischannels( Message& msg );		//[319] WHOIS
	static void rpl_channelmodeis( Message& msg );		//[324] MODE
	static void rpl_notopic( Message& msg );			//[331] JOIN,TOPIC
	static void rpl_topic( Message& msg );				//[332] JOIN,TOPIC
	static void rpl_banlist( Message& msg );			//[367] MODE
	static void rpl_endofbanlist( Message& msg );		//[368] MODE
	static void rpl_usersstart( Message& msg );			//[392] WHOIS
	static void rpl_endofusers( Message& msg );			//[394] WHOIS
	static void rpl_nousers( Message& msg );			//[395] WHOIS
	static void err_nosuchserver( Message& msg);		//[402] USERS,WHOIS
	static void err_nosuchchannel( Message& msg );		//[403] KICK
	static void err_noorigin( Message& msg );			//[409] PING
	static void err_nonicknamegiven( Message& msg);		//[431] NICK
	static void err_erroneusnickname( Message& msg);	//[432] NICK
	static void err_nicknameinuse( Message& msg);		//[433] NICK
	static void err_nickcollision( Message& msg);		//[436] NICK
	static void err_notonchannel( Message& msg );		//[442] KICK,TOPIC
	static void err_userdisabled( Message& msg );		//[446] USERS
	static void err_needmoreparams( Message& msg );		//[461] KICK,TOPIC,USERS_MSG
	static void err_alreadyregistered( Message& msg );	//[462] USERS_MSG
	static void	err_passwdmismatch( Message& msg );		//[464] USERS_MSG
	static void err_badchanmask( Message& msg );		//[476] KICK
	static void err_chanoprivsneeded( Message& msg );	//[482] KICK,TOPIC
	
	/*------------------COMMANDS_UTILS------------------*/	
	static void send_to_clients( t_client_ptr_list list_of_client, string command_in);
	static void send_to_channels(t_channel_ptr_list list_of_chan, string output);

};

} // namespace irc end bracket

#endif
