/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/12 19:58:20 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"

using namespace irc;
using std::cout;
using std::cerr;
using std::endl;

Server*			CommandManager::_server = NULL;
Database*		CommandManager::_database = NULL;
t_command_map	CommandManager::_command_map = t_command_map();
t_reply_map		CommandManager::_reply_map = t_reply_map();

CommandManager::CommandManager( void ) // main constructor
{
	_init_command_map();
	_init_reply_map();
}

CommandManager::CommandManager( Database* database )	// copy constructor
{
	_init_command_map();
	_init_reply_map();
	set_database(database);
}

CommandManager::~CommandManager( void )									// destructor
{
	_reply_map.clear();
	_command_map.clear();
}

/*--------------------------PRIVATE-MEMBER-FUNCTIONS--------------------------*/

void	CommandManager::_init_command_map( void )
{
	_command_map.insert(std::make_pair(string("NICK"), cmd_nick));
	_command_map.insert(std::make_pair(string("USER"), cmd_user));
	_command_map.insert(std::make_pair(string("WHOIS"), cmd_whois));
	_command_map.insert(std::make_pair(string("PING"), cmd_ping));
	_command_map.insert(std::make_pair(string("JOIN"), cmd_join));
	_command_map.insert(std::make_pair(string("QUIT"), cmd_quit));
	_command_map.insert(std::make_pair(string("PRIVMSG"), cmd_privmsg));
	_command_map.insert(std::make_pair(string("KICK"), cmd_kick));
	_command_map.insert(std::make_pair(string("MODE"), cmd_mode));

}

void	CommandManager::_init_reply_map( void )
{
	// _reply_map.insert(std::make_pair(ERR_NONICKNAMEGIVEN, err_nonicknamegiven));
	_reply_map.insert(std::make_pair(ERR_ERRONEUSNICKNAME, err_erroneusnickname));
	_reply_map.insert(std::make_pair(ERR_NICKNAMEINUSE, err_nicknameinuse));
	// _reply_map.insert(std::make_pair(ERR_NICKCOLLISION, err_nickcollision));
	_reply_map.insert(std::make_pair(ERR_NOSUCHSERVER, err_nosuchserver));
	// _reply_map.insert(std::make_pair(ERR_USERDISABLED, *err_userdisabled));
	// _reply_map.insert(std::make_pair(ERR_NOUSERS, rpl_nousers));
	// _reply_map.insert(std::make_pair(RPL_USERSSTART, rpl_usersstart));
	// _reply_map.insert(std::make_pair(RPL_ENDOFUSERS, rpl_endofusers));
	_reply_map.insert(std::make_pair(ERR_NEEDMOREPARAMS, err_needmoreparams));
	_reply_map.insert(std::make_pair(ERR_ALREADYREGISTERED, err_alreadyregistered));
	_reply_map.insert(std::make_pair(RPL_WELCOME, rpl_welcome));
	_reply_map.insert(std::make_pair(RPL_WHOISUSER, rpl_whoisuser));
	_reply_map.insert(std::make_pair(RPL_WHOISSERVER, rpl_whoisserver));
	_reply_map.insert(std::make_pair(RPL_WHOISOPERATOR, rpl_whoisoperator));
	_reply_map.insert(std::make_pair(RPL_ENDOFWHOIS, rpl_endofwhois));
	_reply_map.insert(std::make_pair(RPL_WHOISCHANNELS, rpl_whoischannels));
	_reply_map.insert(std::make_pair(ERR_NOORIGIN, err_noorigin));
	_reply_map.insert(std::make_pair(RPL_NOTOPIC, rpl_notopic));
	_reply_map.insert(std::make_pair(RPL_TOPIC, rpl_topic));	
	_reply_map.insert(std::make_pair(ERR_NOSUCHCHANNEL, err_nosuchchannel));
	_reply_map.insert(std::make_pair(ERR_BADCHANMASK, err_badchanmask));
	_reply_map.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, err_chanoprivsneeded));
	_reply_map.insert(std::make_pair(ERR_NOTONCHANNEL, err_notonchannel));
	_reply_map.insert(std::make_pair(RPL_CHANNELMODEIS, rpl_channelmodeis));
	_reply_map.insert(std::make_pair(RPL_BANLIST, rpl_banlist));
	_reply_map.insert(std::make_pair(RPL_ENDOFBANLIST, rpl_endofbanlist));


	//_command_map.insert(std::make_pair(string("NOM_DE_COMMANDE"), cmd_join));

}

/*-----------------------GETTERS----------------------*/

t_cmd_function_ptr		CommandManager::get_command_ptr( string name )
{
	t_command_map::iterator it;

	for (it = _command_map.begin(); it != _command_map.end(); it++)
	{
		if ((*it).first == name)
			return (*it).second;
	}
	return NULL;
}

t_reply_function_ptr	CommandManager::get_reply_ptr( int code )
{
	t_reply_map::iterator it;

	for (it = _reply_map.begin(); it != _reply_map.end(); it++)
	{
		if ((*it).first == code)
			return (*it).second;
	}
	return NULL;
}

/*-----------------------SETTERS----------------------*/

void	CommandManager::set_server( Server* server )
{
	_server = server;
}

void	CommandManager::set_database( Database* database )
{
	_database = database;
}

/*---------------COMMANDS-FUNCTIONS---------------*/

void	CommandManager::execute_commands( Client& client )
{
	string&	buffin = client.get_buff(BUFFIN);
	size_t	start = 0;
	size_t	next = 0;
	
	Message	msg(&client);
	t_cmd_function_ptr command;

	while ((next = buffin.find("\r\n", start)) != string::npos)
	{
		msg = Message(&client);
		msg.append_in(buffin.substr(start, next - start));
		command = get_command_ptr(msg[0]);
		if (command)
		{
			command(msg);
			client.append_buff(BUFFOUT, msg.get_message_out());
		}
		msg.clear_all();
		start = next + 2;
	}
	return ;
}

void	CommandManager::execute_commands_registration( Client& client )
{
	string&	buffin = client.get_buff(BUFFIN);
	size_t	start = 0;
	size_t	last = 0;
	size_t	next = 0;
	size_t	len	= buffin.length();
	
	Message	msg(&client);
	t_cmd_function_ptr command;
	while ((next = buffin.find("\r\n", start)) != string::npos)
	{
		msg = Message(&client);
		msg.append_in(buffin.substr(start, next - start));
		if (msg[0].compare("NICK") == 0 || msg[0].compare("USER") == 0 || msg[0].compare("PASS") == 0)
		{
			command = get_command_ptr(msg[0]);
			if (command)
			{
				command(msg);
				client.append_buff(BUFFOUT, msg.get_message_out());
			}
		}
		msg.clear_all();
		start = next + 2;
		if (start >= len)
			return ;
	}
	return ;
}
