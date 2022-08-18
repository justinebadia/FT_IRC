
#pragma once
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

	/*-----------------COMMANDS-FUNCTIONS-----------------*/
	static void	execute_commands( Client& client );
	static void	execute_commands_registration( Client& client );

	// COMMANDS []WARNING[] TEMPORARILY IN ALPHABETICAL ORDER
	static void cmd_invite( Message& msg );
	static void cmd_join( Message& msg );
	static void process_single_join( Message& msg );
	static void	cmd_kick( Message& msg );
	static void	cmd_kill( Message& msg );
	static void	cmd_list( Message& msg );
	static void process_single_list( Message& msg );
	static void cmd_mode( Message& msg );
	static void cmd_names( Message& msg );
	static void process_single_names( Message& msg );
	static void	cmd_nick( Message& msg );
	static void	cmd_notice( Message& msg );
	static void	cmd_oper( Message& msg );
	static void	cmd_part( Message& msg );
	static void process_single_part( Message& msg );
	static void	cmd_pass( Message& msg );
	static void cmd_ping( Message& msg );
	static void cmd_privmsg( Message& msg );
	static void cmd_quit( Message& msg );
	static void	cmd_topic( Message& msg );
	static void	cmd_user( Message& msg );
	static void cmd_who( Message& msg );
	static void	cmd_whois( Message& msg );

	/*------------------REPLIES-FUNCTIONS-----------------*/
	static void run_reply( int code, Message& msg );

	// NUMERIC REPLIES
	static void	rpl_welcome( Message& msg );			//[001] ????
	static void	rpl_away( Message& msg );				//[301] INVITE
	static void	rpl_whoisuser( Message& msg );			//[311] WHOIS
	static void	rpl_whoisserver( Message& msg );		//[312] WHOIS
	static void	rpl_whoisoperator( Message& msg );		//[313] WHOIS
	static void rpl_endofwho( Message& msg );			//[315] WHO
	static void	rpl_endofwhois( Message& msg );			//[318] WHOIS
	static void	rpl_whoischannels( Message& msg );		//[319] WHOIS
	// static void	rpl_liststart( Message& msg );			//[321] LIST   OBSOLETE RFC 2812
	static void rpl_list( Message& msg );				//[322] LIST
	static void	rpl_listend( Message& msg );			//[323] LIST
	static void	rpl_channelmodeis( Message& msg );		//[324] MODE
	static void	rpl_notopic( Message& msg );			//[331] JOIN,TOPIC
	static void	rpl_topic( Message& msg );				//[332] JOIN,TOPIC
	static void	rpl_inviting( Message& msg );			//[341] INVITE
	static void rpl_whoreply( Message& msg );			//[352] WHO
	static void	rpl_namreply( Message& msg );			//[353] NAMES
	static void	rpl_endofnames( Message& msg );			//[366] NAMES
	static void	rpl_banlist( Message& msg );			//[367] MODE
	static void	rpl_endofbanlist( Message& msg );		//[368] MODE
	static void rpl_youreoper( Message& msg );			//[381] OPER
	static void	rpl_usersstart( Message& msg );			//[392] WHOIS
	static void	rpl_endofusers( Message& msg );			//[394] WHOIS
	static void	rpl_nousers( Message& msg );			//[395] WHOIS
	static void	err_nosuchnick( Message& msg);			//[401] INVITE,KILL
	static void	err_nosuchserver( Message& msg);		//[402] LIST,NAMES,USERS,WHOIS
	static void	err_nosuchchannel( Message& msg );		//[403] KICK,PART
	static void	err_noorigin( Message& msg );			//[409] PING
	static void	err_nonicknamegiven( Message& msg);		//[431] NICK
	static void	err_erroneusnickname( Message& msg);	//[432] NICK
	static void	err_nicknameinuse( Message& msg);		//[433] NICK
	static void	err_nickcollision( Message& msg);		//[436] NICK
	static void	err_notonchannel( Message& msg );		//[442] INVITE,KICK,PART,TOPIC
	static void	err_useronchannel( Message& msg );		//[443] INVITE
	static void	err_userdisabled( Message& msg );		//[446] USERS
	static void	err_needmoreparams( Message& msg );		//[461] INVITE,KICK,KILL,PART,TOPIC,USERS_MSG
	static void	err_alreadyregistered( Message& msg );	//[462] USERS_MSG
	static void	err_passwdmismatch( Message& msg );		//[464] USERS_MSG
	static void err_keyset( Message& msg ); 			//[467] MODE_ERR_KEYSET
	static void	err_badchanmask( Message& msg );		//[476] KICK
	static void	err_noprivileges( Message& msg );		//[481] KILL
	static void	err_chanoprivsneeded( Message& msg );	//[482] INVITE,KICK,TOPIC
	static void	err_cantkillserver( Message& msg );		//[483] KILL
	static void err_nooperhost( Message& msg );			//[491] OPER
	
	/*------------------COMMANDS_UTILS------------------*/	
	static void send_to_clients( t_client_ptr_list list_of_client, string command_in);
	static void send_to_channels(t_channel_ptr_list list_of_chan, string output);
	static void handle_o_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg);
	static void handle_k_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg);
	static void handle_b_mode(size_t &pos_it, string& modes, string& params, std::vector<string> parsed, Channel* channel, Message& msg);

};

} // namespace irc end bracket

#endif
