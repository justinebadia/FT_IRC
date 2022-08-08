/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/08 12:48:40 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"
#include "Server.hpp"
#include "numeric_replies.hpp"
#include "replies.hpp"
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
	size_t	last = 0;
	size_t	next = 0;
	size_t	len	= buffin.length();
	
	Message	msg(&client);
	t_cmd_function_ptr command;

	std::cout << GREEN << "BUFFIN : " << RESET<< buffin << GREEN << " FIND = " << buffin.find("\r\n", start) << RESET << std::endl;
	while ((next = buffin.find("\r\n", start)) != string::npos)
	{
		msg = Message(&client);
		msg.append_in(buffin.substr(start, next - start));
		// std::cout << "le msg est : " << msg.get_message_in() << std::endl;
		command = get_command_ptr(msg[0]);
		if (command)
		{
			command(msg);
			client.append_buff(BUFFOUT, msg.get_message_out());
		}
		msg.clear_all();
		start = next + 2;
		if (start >= len)
			return ;
	}
	return ;
}

void	CommandManager::cmd_nick( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();

	if ( !validate_entry(REGEX_NICKNAME, msg[1]) )
	{
		get_reply_ptr(ERR_ERRONEUSNICKNAME)(msg);
		return;
	}
	if ( _database->get_client( msg[1] ) )
	{
		get_reply_ptr(ERR_NICKNAMEINUSE)(msg);
		return;
	}
	client.set_nickname(msg[1]);
	std::cout << "Successfully set the nickname to " << msg[1] << std::endl;
}

void	CommandManager::cmd_user( Message& msg )
{
	Client& client			= *msg.get_client_ptr();

	if(msg[4].empty()) //checking if there is, at least, 5 parameters
	{
		get_reply_ptr(ERR_NEEDMOREPARAMS)(msg);
		return ;
	}
	if (!msg[1].empty())
		client.set_username(msg[1]);
	//if (msg[2].compare(0, msg[2].size(), client.get_hostname()) == 0) est ce qu'on check si le hostname est faux ?
	if (msg[4].find(":", 0) != string::npos)
	{
		client.set_realname(msg.find_realname());
		// std::cout << client.get_realname() << std::endl;

	}	
	// On vérifie le hostname et le servername ?
	// Est ce qu'on doit afficher le timestamp ?? Ou on l'affiche quand le msg est buildé au complet
	
	return ;
}

void CommandManager::cmd_whois( Message & msg )
{
	Client& client			= *msg.get_client_ptr();
	
	if (!msg[2].empty())
	{
		if (msg[2] != _server->get_name())
			get_reply_ptr(ERR_NOSUCHSERVER)(msg);
		return;
	}
	// get_reply_ptr(RPL_WHOISUSER)(msg);
	// get_reply_ptr(RPL_WHOISSERVER)(msg);
	// get_reply_ptr(RPL_WHOISOPERATOR)(msg);
	// get_reply_ptr(RPL_WHOISCHANNELS)(msg);
	//msg.append_out(client.get_nickname() + " " + client.get_hostname() + "\nircname: " + client.get_realname() + "\nserver: " + server.get_name());
	// get_reply_ptr(RPL_ENDOFWHOIS)(msg); //signifie que c'est la fin de la querry WHOIS
	return;
}

/*------------------REPLIES-FUNCTIONS-----------------*/
void	CommandManager::run_reply( int code, Message& msg )
{
	t_reply_function_ptr reply_ptr;

	reply_ptr = get_reply_ptr(code);
	if (reply_ptr)
		reply_ptr(msg);
	else
		std::cout << GREEN << code << " reply function not found" << RESET << std::endl;
	return; 
}


