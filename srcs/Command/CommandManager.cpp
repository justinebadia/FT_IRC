
#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
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
			// WARNING TEMPORARILY IN ALPHABETICAL ORDER
	_command_map.insert(std::make_pair(string("INVITE"), cmd_invite));
	_command_map.insert(std::make_pair(string("JOIN"), cmd_join));
	_command_map.insert(std::make_pair(string("KICK"), cmd_kick));
	_command_map.insert(std::make_pair(string("KILL"), cmd_kill));
	// _command_map.insert(std::make_pair(string("MODE"), cmd_mode));
	_command_map.insert(std::make_pair(string("NOTICE"), cmd_notice));
	_command_map.insert(std::make_pair(string("NICK"), cmd_nick));
	_command_map.insert(std::make_pair(string("OPER"), cmd_oper));
	_command_map.insert(std::make_pair(string("PART"), cmd_part));
	_command_map.insert(std::make_pair(string("PASS"), cmd_pass));
	_command_map.insert(std::make_pair(string("PING"), cmd_ping));
	_command_map.insert(std::make_pair(string("PRIVMSG"), cmd_privmsg));
	_command_map.insert(std::make_pair(string("QUIT"), cmd_quit));
	_command_map.insert(std::make_pair(string("TOPIC"), cmd_topic));
	_command_map.insert(std::make_pair(string("USER"), cmd_user));
	_command_map.insert(std::make_pair(string("WHOIS"), cmd_whois));

}

void	CommandManager::_init_reply_map( void )
{
	_reply_map.insert(std::make_pair(RPL_WELCOME, rpl_welcome));						//[001] ????
	_reply_map.insert(std::make_pair(RPL_AWAY, rpl_away));								//[301] INVITE
	_reply_map.insert(std::make_pair(RPL_WHOISUSER, rpl_whoisuser));					//[311] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISSERVER, rpl_whoisserver));				//[312] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISOPERATOR, rpl_whoisoperator));			//[313] WHOIS
	_reply_map.insert(std::make_pair(RPL_ENDOFWHOIS, rpl_endofwhois));					//[318] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISCHANNELS, rpl_whoischannels));			//[319] WHOIS
	_reply_map.insert(std::make_pair(RPL_CHANNELMODEIS, rpl_channelmodeis));			//[324] MODE
	_reply_map.insert(std::make_pair(RPL_NOTOPIC, rpl_notopic));						//[331] JOIN
	_reply_map.insert(std::make_pair(RPL_TOPIC, rpl_topic));							//[332] JOIN
	_reply_map.insert(std::make_pair(RPL_INVITING, rpl_inviting));						//[341] INVITE
	_reply_map.insert(std::make_pair(RPL_BANLIST, rpl_banlist));						//[367] MODE
	_reply_map.insert(std::make_pair(RPL_ENDOFBANLIST, rpl_endofbanlist));				//[368] MODE
	_reply_map.insert(std::make_pair(RPL_YOUREOPER, rpl_youreoper));					//[381] OPER
	// _reply_map.insert(std::make_pair(RPL_USERSSTART, rpl_usersstart));				//[392] WHOIS
	// _reply_map.insert(std::make_pair(RPL_ENDOFUSERS, rpl_endofusers));				//[394] WHOIS
	// _reply_map.insert(std::make_pair(ERR_NOUSERS, rpl_nousers));						//[395] WHOIS
	_reply_map.insert(std::make_pair(ERR_NOSUCHNICK, err_nosuchnick));					//[401] INVITE
	_reply_map.insert(std::make_pair(ERR_NOSUCHSERVER, err_nosuchserver));				//[402] USERS,WHOIS
	_reply_map.insert(std::make_pair(ERR_NOSUCHCHANNEL, err_nosuchchannel));			//[403] KICK,PART
	_reply_map.insert(std::make_pair(ERR_NOORIGIN, err_noorigin));						//[409] PING
	// _reply_map.insert(std::make_pair(ERR_NONICKNAMEGIVEN, err_nonicknamegiven));		//[431] NICK
	_reply_map.insert(std::make_pair(ERR_ERRONEUSNICKNAME, err_erroneusnickname));		//[432] NICK
	_reply_map.insert(std::make_pair(ERR_NICKNAMEINUSE, err_nicknameinuse));			//[433] NICK
	// _reply_map.insert(std::make_pair(ERR_NICKCOLLISION, err_nickcollision));			//[436] NICK
	_reply_map.insert(std::make_pair(ERR_NOTONCHANNEL, err_notonchannel));				//[442] INVITE,KICK,PART,TOPIC
	_reply_map.insert(std::make_pair(ERR_USERONCHANNEL, err_useronchannel));			//[443] INVITE
	// _reply_map.insert(std::make_pair(ERR_USERDISABLED, *err_userdisabled));			//[446] USERS
	_reply_map.insert(std::make_pair(ERR_NEEDMOREPARAMS, err_needmoreparams));			//[461] INVITE,KICK,PART,TOPIC,USERS_MSG
	_reply_map.insert(std::make_pair(ERR_ALREADYREGISTERED, err_alreadyregistered));	//[462] USERS_MSG
	_reply_map.insert(std::make_pair(ERR_PASSWDMISMATCH, err_passwdmismatch));			//[464] USERS_MSG,OPER
	_reply_map.insert(std::make_pair(ERR_KEYSET, err_keyset));							//[467] MODE_KEY_SET
	_reply_map.insert(std::make_pair(ERR_BADCHANMASK, err_badchanmask));				//[476] KICK
	_reply_map.insert(std::make_pair(ERR_NOPRIVILEGES, err_noprivileges));				//[481] KILL
	_reply_map.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, err_chanoprivsneeded));		//[482] INVITE,KICK,TOPIC
	_reply_map.insert(std::make_pair(ERR_CANTKILLSERVER, err_cantkillserver));			//[483] KILL
	_reply_map.insert(std::make_pair(ERR_NOOPERHOST, err_nooperhost));					//[491] OPER
	

}

bool	CommandManager::_is_unregistered_allowed( const string& cmd_name )
{
	if (cmd_name.compare("NICK") == 0 
		|| cmd_name.compare("USER") == 0
		|| cmd_name.compare("PASS") == 0
		|| cmd_name.compare("QUIT") == 0)
	{
		return true;
	}
	return false;
}


/*----------------------------------GETTERS-----------------------------------*/
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

/*----------------------------------SETTERS-----------------------------------*/

void	CommandManager::set_server( Server* server )
{
	_server = server;
}

void	CommandManager::set_database( Database* database )
{
	_database = database;
}

/*-----------------------------COMMANDS-FUNCTIONS-----------------------------*/

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
			if (msg.get_client_ptr())
				client.append_buff(BUFFOUT, msg.get_message_out());
		}
		msg.clear_all();
		start = next + 2;
	}
	if (start != string::npos)
		buffin.erase(0, start);
	return ;
}

void	CommandManager::execute_commands_registration( Client& client )
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
		if (_is_unregistered_allowed(msg[0]))
		{
			if (client.is_password_validated() || msg[0] == "PASS" || msg[0] == "QUIT")
			{
				command = get_command_ptr(msg[0]);
				if (command)
				{
					command(msg);
					client.append_buff(BUFFOUT, msg.get_message_out());
				}
			}
		}
		msg.clear_all();
		start = next + 2;
	}
	if (start != string::npos)
		buffin.erase(0, start);
	return ;
}
