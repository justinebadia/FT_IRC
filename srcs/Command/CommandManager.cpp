
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
	_command_map.insert(std::make_pair(string("ERROR"), cmd_error));
	_command_map.insert(std::make_pair(string("INVITE"), cmd_invite));
	_command_map.insert(std::make_pair(string("JOIN"), cmd_join));
	_command_map.insert(std::make_pair(string("KICK"), cmd_kick));
	_command_map.insert(std::make_pair(string("KILL"), cmd_kill));
	_command_map.insert(std::make_pair(string("LIST"), cmd_list));
	_command_map.insert(std::make_pair(string("MODE"), cmd_mode));
	_command_map.insert(std::make_pair(string("MOTD"), cmd_motd));
	_command_map.insert(std::make_pair(string("NOTICE"), cmd_notice));
	_command_map.insert(std::make_pair(string("NAMES"), cmd_names));
	_command_map.insert(std::make_pair(string("NICK"), cmd_nick));
	_command_map.insert(std::make_pair(string("OPER"), cmd_oper));
	_command_map.insert(std::make_pair(string("PART"), cmd_part));
	_command_map.insert(std::make_pair(string("PASS"), cmd_pass));
	_command_map.insert(std::make_pair(string("PING"), cmd_ping));
	_command_map.insert(std::make_pair(string("PRIVMSG"), cmd_privmsg));
	_command_map.insert(std::make_pair(string("QUIT"), cmd_quit));
	_command_map.insert(std::make_pair(string("SQUIT"), cmd_squit));
	_command_map.insert(std::make_pair(string("TOPIC"), cmd_topic));
	_command_map.insert(std::make_pair(string("USER"), cmd_user));
	_command_map.insert(std::make_pair(string("WHO"), cmd_who));
	_command_map.insert(std::make_pair(string("WHOIS"), cmd_whois));
}

void	CommandManager::_init_reply_map( void )
{
	_reply_map.insert(std::make_pair(RPL_WELCOME, rpl_welcome));						//[001] ????
	_reply_map.insert(std::make_pair(RPL_WHOISUSER, rpl_whoisuser));					//[311] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISSERVER, rpl_whoisserver));				//[312] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISOPERATOR, rpl_whoisoperator));			//[313] WHOIS
	_reply_map.insert(std::make_pair(RPL_ENDOFWHO, rpl_endofwho));						//[315] WHO
	_reply_map.insert(std::make_pair(RPL_ENDOFWHOIS, rpl_endofwhois));					//[318] WHOIS
	_reply_map.insert(std::make_pair(RPL_WHOISCHANNELS, rpl_whoischannels));			//[319] WHOIS
	_reply_map.insert(std::make_pair(RPL_LIST, rpl_list));								//[322] LIST
	_reply_map.insert(std::make_pair(RPL_LISTEND, rpl_listend));						//[323] LIST
	_reply_map.insert(std::make_pair(RPL_CHANNELMODEIS, rpl_channelmodeis));			//[324] MODE
	_reply_map.insert(std::make_pair(RPL_NOTOPIC, rpl_notopic));						//[331] JOIN
	_reply_map.insert(std::make_pair(RPL_TOPIC, rpl_topic));							//[332] JOIN
	_reply_map.insert(std::make_pair(RPL_INVITING, rpl_inviting));						//[341] INVITE
	_reply_map.insert(std::make_pair(RPL_WHOREPLY, rpl_whoreply));						//[352] WHO
	_reply_map.insert(std::make_pair(RPL_NAMREPLY, rpl_namreply));						//[353] NAMES
	_reply_map.insert(std::make_pair(RPL_ENDOFNAMES, rpl_endofnames));					//[366] NAMES
	_reply_map.insert(std::make_pair(RPL_BANLIST, rpl_banlist));						//[367] MODE
	_reply_map.insert(std::make_pair(RPL_ENDOFBANLIST, rpl_endofbanlist));				//[368] MODE
	_reply_map.insert(std::make_pair(RPL_ENDOFBANLIST, rpl_endofbanlist));				//[368] MODE
	_reply_map.insert(std::make_pair(RPL_MOTDSTART, rpl_motdstart));					//[375] MOTD
	_reply_map.insert(std::make_pair(RPL_MOTD, rpl_motd));								//[372] MOTD
	_reply_map.insert(std::make_pair(RPL_ENDOFMOTD, rpl_endofmotd));					//[376] MOTD
	_reply_map.insert(std::make_pair(RPL_YOUREOPER, rpl_youreoper));					//[381] OPER
	_reply_map.insert(std::make_pair(ERR_NOSUCHNICK, err_nosuchnick));					//[401] INVITE,KILL,PRIVMSG,WHOIS
	_reply_map.insert(std::make_pair(ERR_NOSUCHSERVER, err_nosuchserver));				//[402] LIST,NAMES,PING,USERS,WHO,WHOIS
	_reply_map.insert(std::make_pair(ERR_NOSUCHCHANNEL, err_nosuchchannel));			//[403] JOIN,PRIVMSG,KICK,PART
	_reply_map.insert(std::make_pair(ERR_CANNOTSENDTOCHAN, err_cannotsendtochan));		//[404] PRIVMSG
	_reply_map.insert(std::make_pair(ERR_TOOMANYCHANNELS, err_toomanychannels));		//[405] JOIN
	_reply_map.insert(std::make_pair(ERR_NOORIGIN, err_noorigin));						//[409] PING
	_reply_map.insert(std::make_pair(ERR_NORECIPIENT, err_norecipient));				//[411] PRIVMSG
	_reply_map.insert(std::make_pair(ERR_NOTEXTTOSEND, err_notexttosend));				//[412] PRIVMSG
	_reply_map.insert(std::make_pair(ERR_NONICKNAMEGIVEN, err_nonicknamegiven));		//[431] NICK,WHOIS
	_reply_map.insert(std::make_pair(ERR_ERRONEUSNICKNAME, err_erroneusnickname));		//[432] NICK
	_reply_map.insert(std::make_pair(ERR_NICKNAMEINUSE, err_nicknameinuse));			//[433] NICK
	_reply_map.insert(std::make_pair(ERR_USERNOTINCHANNEL, err_usernotinchannel));		//[441] KICK
	_reply_map.insert(std::make_pair(ERR_NOTONCHANNEL, err_notonchannel));				//[442] INVITE,KICK,PART,TOPIC
	_reply_map.insert(std::make_pair(ERR_USERONCHANNEL, err_useronchannel));			//[443] INVITE
	_reply_map.insert(std::make_pair(ERR_NEEDMOREPARAMS, err_needmoreparams));			//[461] INVITE,JOIN,KICK,OPER,PART,PASS,TOPIC,USERS_MSG
	_reply_map.insert(std::make_pair(ERR_ALREADYREGISTERED, err_alreadyregistered));	//[462] PASS,USERS_MSG
	_reply_map.insert(std::make_pair(ERR_PASSWDMISMATCH, err_passwdmismatch));			//[464] OPER,PASS
	_reply_map.insert(std::make_pair(ERR_KEYSET, err_keyset));							//[467] MODE_KEY_SET
	_reply_map.insert(std::make_pair(ERR_CHANNELISFULL, err_channelisfull));			//[471] JOIN
	_reply_map.insert(std::make_pair(ERR_UNKNOWNMODE, err_unknownmode));				//[472] MODE
	_reply_map.insert(std::make_pair(ERR_INVITEONLYCHAN, err_inviteonlychan));			//[473] JOIN
	_reply_map.insert(std::make_pair(ERR_BANNEDFROMCHAN, err_bannedfromchan));			//[474] JOIN
	_reply_map.insert(std::make_pair(ERR_BADCHANNELKEY, err_badchannelkey));			//[475] JOIN
	_reply_map.insert(std::make_pair(ERR_BADCHANMASK, err_badchanmask));				//[476] JOIN,KICK
	_reply_map.insert(std::make_pair(ERR_NOPRIVILEGES, err_noprivileges));				//[481] KILL
	_reply_map.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, err_chanoprivsneeded));		//[482] INVITE,KICK,TOPIC
	_reply_map.insert(std::make_pair(ERR_CANTKILLSERVER, err_cantkillserver));			//[483] KILL
	_reply_map.insert(std::make_pair(ERR_NOOPERHOST, err_nooperhost));					//[491] OPER
	

}

bool	CommandManager::_is_allowed( const string& cmd_name, Client* client )
{
	if (client->is_registered() == true)
		return true;
	if (cmd_name.compare("PASS") == 0 || cmd_name.compare("QUIT") == 0)
		return true;
	if (client->is_password_validated() && (cmd_name.compare("NICK") == 0 || cmd_name.compare("USER") == 0))
		return true;
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

void	CommandManager::execute_commands( Client* client )
{
	string&	buffin = client->get_buff(BUFFIN);
	size_t	start = 0;
	size_t	next = 0;
	
	Message	msg(client);
	t_cmd_function_ptr command;
	while ((next = buffin.find("\r\n", start)) != string::npos)
	{
		msg = Message(client);
		msg.append_in(buffin.substr(start, next - start));
		if (_is_allowed(msg[0], client) == true)
		{
			command = get_command_ptr(msg[0]);
			if (command)
			{
				command(msg);
				client->append_buff(BUFFOUT, msg.get_message_out());
				if (client->is_registered() == false)
					_server->check_registration(client);
			}
		}
		msg.clear_all();
		start = next + 2;
	}
	if (start != string::npos)
		buffin.erase(0, start);
	return ;
}
