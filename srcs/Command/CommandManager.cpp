/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/10 14:34:18 by jbadia           ###   ########.fr       */
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

void CommandManager::cmd_join( Message& msg )
{
	t_client_ptr_list	chan_memberlist;
	Channel* 			channel;
	string topic;
	Client& client = *msg.get_client_ptr();
	
	if (msg[1].empty())
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return;
	}
	if (_database->get_channel_count() >= MAX_CHANNELS)
	{
		run_reply(ERR_TOOMANYCHANNELS, msg);
		return;
	}
	channel = _database->get_channel(msg[1]);
	chan_memberlist = _database->get_clients_in_channel(channel);
	if (!channel)
	{
		_database->add_channel_list(Channel(string(msg[1]), msg.get_client_ptr()));
		channel = _database->get_channel(msg[1]);
	}
	else if (chan_memberlist.size() >= MAX_CLIENT_PER_CHAN)
	{
		run_reply(ERR_CHANNELISFULL, msg);
		return;
	}
	else if (channel->is_banned(msg.get_client_ptr()))
	{
		run_reply(ERR_BANNEDFROMCHAN, msg);
		return ;
	}
	else if (channel->get_is_invite_only())
	{
		run_reply(ERR_INVITEONLYCHAN, msg);
		return ;
	}
	_database->add_client_to_channel(msg.get_client_ptr(), channel);
	topic = channel->get_topic();
	run_reply(RPL_TOPIC, msg);
	client.append_buff(BUFFOUT, client.get_prefix() + "JOIN " + msg[1] + CRLF);
	// WARNING ERR_BADCHANMASK
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL??
	return;
}

void	CommandManager::cmd_nick( Message& msg )
{ 
	Client& client			= *msg.get_client_ptr();

	if ( !validate_entry(REGEX_NICKNAME, msg[1]) )
	{
		run_reply(ERR_ERRONEUSNICKNAME, msg);
		return;
	}
	if ( _database->get_client( msg[1] ) )
	{
		run_reply(ERR_NICKNAMEINUSE, msg);
		return;
	}
	client.set_nickname(msg[1]);
	client.set_registration_flags(Client::NICK_SET);
	Server::log(string() + GREEN + "Successfully set the nickname to " + msg[1] + RESET);
	client.append_buff(BUFFOUT, client.get_prefix() + "NICK " + msg[1] + CRLF);
}

void CommandManager::cmd_privmsg( Message& msg ) // WARNING done minimally for channel testing
{
	t_client_ptr_list	chan_memberlist;
	Channel* 			channel;
	Client*				client;
	string				prefix;

	if (msg[1].empty())
	{
		// run_reply(ERR_NORECIPIENT, msg);
		return;
	}
	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		channel = _database->get_channel(msg[1]);
		if (channel)
		{
			chan_memberlist = _database->get_clients_in_channel(channel);
			send_to_clients(chan_memberlist, ":" + msg.get_client_ptr()->get_nickname() + " PRIVMSG " + channel->get_name() + " :" + msg.get_colon() + "\n");
		}
	}
	else
	{
		client = _database->get_client(msg[1]);
		if (client)
			client->append_buff(BUFFOUT, client->get_prefix() + " PRIVMSG " + client->get_nickname() + " :" + msg.get_colon() + "\n");
	}
	
	return;
}

void	CommandManager::cmd_user( Message& msg )
{
	Client& client			= *msg.get_client_ptr();

	if(msg[4].empty() || msg.get_colon().empty()) //checking if there is, at least, 5 parameters
	{
		run_reply(ERR_NEEDMOREPARAMS, msg);
		return ;
	}
	if (!msg[1].empty())
		client.set_username(msg[1]);
	if ( !msg.get_colon().empty() )
		client.set_realname(msg.get_colon());
	client.set_hostname(msg[3]);
	client.set_registration_flags(Client::USER_SET);
	Server::log(string() + GREEN + "Successfully set the username to " + msg.get_colon() + RESET);
	return ;
}

void CommandManager::cmd_whois( Message & msg )
{
	if (!msg[2].empty())
	{
		if (msg[2] != _server->get_name())
			get_reply_ptr(ERR_NOSUCHSERVER)(msg);
		return;
	}
	run_reply(RPL_WHOISUSER, msg);
	run_reply(RPL_WHOISSERVER, msg);
	run_reply(RPL_WHOISOPERATOR, msg);
	//get_reply_ptr(RPL_WHOISCHANNELS)(msg); WARNING A FAIRE
	run_reply(RPL_ENDOFWHOIS, msg); //signifie que c'est la fin de la querry WHOIS

	return;
}

void CommandManager::cmd_ping( Message& msg )
{
	if (msg[1].empty())
			run_reply(ERR_NOORIGIN, msg);
	else if (msg[1] != _server->get_name() && !_database->get_client(msg[1]))
			run_reply(ERR_NOSUCHSERVER, msg);
	else
		msg.append_out(":127.0.0.1 PONG " + msg.get_client_ptr()->get_nickname() + " :127.0.0.1");
		
	return;
}


void CommandManager::cmd_quit( Message& msg )
{
	string				nickname;
	Client*				client	= msg.get_client_ptr();
	t_channel_ptr_list	chan_list = _database->get_channel_list_of_client(client);

	if (!client)
		return;
	nickname = string(client->get_nickname());
	_database->remove_client_list(client);
	cout << "Amount of channels client is in : " << chan_list.size() << endl;
	if (!msg[1].empty())
		send_to_channels(chan_list, client->get_nickname() + " PRIVMSG " + nickname + " :QUIT:" + msg[1] + "\n");	
	else
		send_to_channels(chan_list, "PRIVMSG " + nickname + " :QUIT \n");
}



