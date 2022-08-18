

#include "CommandManager.hpp"

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Database.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"

namespace irc 
{

void	CommandManager::run_reply( int code, Message& msg )
{
	t_reply_function_ptr reply_ptr;

	reply_ptr = get_reply_ptr(code);
	if (reply_ptr)
	{
		reply_ptr(msg);
		msg.append_out("\r\n");
	}
	else
		std::cout << GREEN << code << " reply function not found" << RESET << std::endl;
	return; 
}

/*=[000-----------------------------------------------------------------------------------------------------------------------------------------099]=*/

void	CommandManager::rpl_welcome( Message& msg )
{
	Client& client = *msg.get_client_ptr();

	msg.append_out(": 001 " + client.get_nickname() + " :Welcome to the Internet Relay Network " + client.get_full_id());
}

/*=[100-----------------------------------------------------------------------------------------------------------------------------------------199]=*/
/*=[300-----------------------------------------------------------------------------------------------------------------------------------------399]=*/
void	CommandManager::rpl_whoisuser( Message& msg)
{
	Client& client = *msg.get_client_ptr();
	Client*	target = _database->get_client(msg[1]);
	string	target_info;

	if (!target)
		return ;
	Server::log(target->get_realname());
	target_info.append(target->get_nickname() + " " + target->get_username() + " " + target->get_client_ip() + " * :" + target->get_realname());
	msg.append_out(": 311 " + client.get_nickname() + " " + target_info);
}

void	CommandManager::rpl_whoisserver( Message& msg )
{
	Client& client = *msg.get_client_ptr();
	Client*	target = _database->get_client(msg[1]);
	string	target_info;

	if (!target)
		return ;
	target_info.append(target->get_nickname() + " " + _server->get_server_ip() + " :Tobastine's server"); 
	msg.append_out(": 312 " + client.get_nickname() + " " + target_info);
}

void	CommandManager::rpl_whoisoperator( Message& msg )
{
	Client& client = *msg.get_client_ptr();
	Client*	target = _database->get_client(msg[1]);

	if (!target)
		return ;
	msg.append_out(": 313 " + client.get_nickname() + " " + target->get_nickname() + " :is an IRC operator");
}

void 	CommandManager::rpl_endofwho( Message& msg )
{
	Client& client = *msg.get_client_ptr();

	msg.append_out(": 315 " + client.get_nickname() + " " + msg[1] + " :End of WHO list");
}

void	CommandManager::rpl_endofwhois( Message& msg )
{
	Client& client = *msg.get_client_ptr();
	Client*	target = _database->get_client(msg[1]);

	if (!target)
		return ;
	msg.append_out(": 318 " + client.get_nickname() + " " + target->get_nickname() + " :End of /WHOIS list");
}

void	CommandManager::rpl_whoischannels( Message& msg )
{
	Client* client = msg.get_client_ptr();
	Client*	target = _database->get_client(msg[1]);
	t_channel_ptr_list channels = _database->get_channel_list_of_client(client);
	string channels_param;
	t_channel_ptr_list::iterator it;
 
	for (it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->is_chanop(client) || (*it)->is_owner(client))
		{
			channels_param += "@";
			channels_param += (*it)->get_name();
		}
		else 
		{
			channels_param += (*it)->get_name();
		}
		channels_param += " ";
	}
	msg.append_out(": 319 " + client->get_nickname() + " " + target->get_nickname() + " " + ":" + channels_param);
		
}

void	CommandManager::rpl_list( Message& msg )
{
	Channel* channel = _database->get_channel(msg[1]);
	Client*	client = msg.get_client_ptr();

	if (!channel)
		return;
	string	names = ": 322 " + client->get_nickname() + " " + channel->get_name() +  " = :" + channel->get_topic();

	msg.append_out(names);
}

void	CommandManager::rpl_listend( Message& msg )
{
	Client* client = msg.get_client_ptr();

	msg.append_out(": 323 " + client->get_nickname() + " :End of LIST");
}

void	CommandManager::rpl_channelmodeis( Message& msg )
{
	Client* client = msg.get_client_ptr();

	msg.append_out(": 324 " + client->get_nickname() + " " + msg[1]);
	if (msg.get_param_count() == 1)
	{
		if (!_database->get_channel(msg[1])->get_password().empty())
			msg.append_out(" +" + _database->get_channel(msg[1])->get_mode_str() + " " + _database->get_channel(msg[1])->get_password());
		else
			msg.append_out(" +" + _database->get_channel(msg[1])->get_mode_str());
	}
	else
		msg.append_out(" " + msg.get_mode_rpl());
}

void	CommandManager::rpl_notopic( Message& msg )
{
	Client* client = msg.get_client_ptr();

	msg.append_out(": 331 " + client->get_nickname() + " " + msg[1] + " :No topic is set");
}

void	CommandManager::rpl_topic( Message& msg )
{
	Client* client = msg.get_client_ptr();

	msg.append_out(": 332 " + client->get_nickname() + " " + msg[1] + " :" + _database->get_channel(msg[1])->get_topic());
}

void	CommandManager::rpl_inviting( Message& msg )
{
	Client* client = msg.get_client_ptr();

	msg.append_out(": 341 " + client->get_nickname() + " " + msg[2] + " " + msg[1]);
}

void CommandManager::rpl_whoreply( Message& msg )
{
	Client* client = msg.get_client_ptr(); 
	string reply;

	if (msg.get_param_count() >= 1)
	{
		if (msg[1][0] == '&' || msg[1][0] == '#')
		{
			Channel* channel = _database->get_channel(msg[1]);
			t_channel_memberlist member_list = channel->get_memberlist();
			t_channel_memberlist::iterator it = member_list.begin();
			t_channel_memberlist::iterator ite = member_list.end();

			for (; it != ite; it++)
			{
				reply = channel->get_name() + " " + (*it).first->get_username() + " " + (*it).first->get_client_ip() + " " + _server->get_server_ip() + " " + (*it).first->get_nickname() + " * :0 " + (*it).first->get_realname();
				msg.append_out(": 352 " + client->get_nickname()  + " " + reply + " ");
				msg.append_out(CRLF);
				run_reply(RPL_ENDOFWHO, msg);
			}
		}
		else
		{
			Client* target = _database->get_client(msg[1]);
			if (target)
				msg.append_out(": 352 " + client->get_nickname() + " * " + target->get_username() + " " + target->get_client_ip() + " " + _server->get_server_ip() + " " + target->get_nickname() + " * :0 " + target->get_realname());
		}
	}
	else 
	{
		t_client_ptr_list client_list = _database->get_client_ptr_list();
		t_client_ptr_list::const_iterator it = client_list.begin();
		t_client_ptr_list::const_iterator ite = client_list.end();
		for (; it != ite; it++)
		{
			msg.append_out(": 352 " + client->get_nickname() + " * " + (*it)->get_username() + " " + (*it)->get_client_ip() + " " + _server->get_server_ip() + " * :0" + (*it)->get_realname());
			msg.append_out(CRLF);
			run_reply(RPL_ENDOFWHO, msg);
			
		}
	}
}
	
void	CommandManager::rpl_namreply( Message& msg )
{
	Channel* channel = _database->get_channel(msg[1]);
	if (!channel)
		return;
	Client*	client = msg.get_client_ptr();
	string	names = ": 353 " + client->get_nickname() + " = " + channel->get_name() +  " :";

	t_channel_memberlist memberlist = channel->get_memberlist();
	t_channel_memberlist::iterator it = memberlist.begin();
	t_channel_memberlist::iterator ite = memberlist.end();
	for (; it != ite;)
	{
		names += ((*it).first->get_nickname());
		if (++it != ite)
			names += " ";
	}
	msg.append_out(names);
}

void	CommandManager::rpl_endofnames( Message& msg )
{
	Channel* channel = _database->get_channel(msg[1]);
	Client*	client = msg.get_client_ptr();
	if (!channel)
		msg.append_out(": 366 " + client->get_nickname() + " * :End of NAMES list");
	else
		msg.append_out(": 366 " + client->get_nickname() + " " + channel->get_name() + " :End of NAMES list");
}

void	CommandManager::rpl_banlist( Message& msg )
{
	Channel* channel = _database->get_channel(msg[1]);
	Client*	client = msg.get_client_ptr();

	if (channel && !channel->get_banmask_list().empty())
	{
		t_mask_list ban_mask = channel->get_banmask_list();
		t_mask_list::iterator it = ban_mask.begin();
		t_mask_list::iterator ite = ban_mask.end();

		for (; it != ite; it++)
			msg.append_out(": 367 " + client->get_nickname() + " " + msg[1] + " " + (*it)); 
	}
	else
		msg.append_out(": 367 " + client->get_nickname() + " " + msg[1]); 

}

void	CommandManager::rpl_endofbanlist( Message& msg )
{
	Client*	client = msg.get_client_ptr();

	msg.append_out(": 368 " + client->get_nickname() + " " + msg[1] + " :End of channel ban list");
}

void	CommandManager::rpl_youreoper( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	
	msg.append_out(": 381 " + client->get_nickname() + " :You are now an IRC operator");
}

/*=[400-----------------------------------------------------------------------------------------------------------------------------------------499]=*/

void	CommandManager::err_nosuchnick( Message& msg)
{
	Client*	client = msg.get_client_ptr();

	string err_msg = ": 401 " + client->get_nickname() + " " + msg[1] + " :No such nick/channel";
	msg.append_out(err_msg);
}

/*"<server name> :No such server"*/
void	CommandManager::err_nosuchserver( Message& msg)
{
	Client*	client = msg.get_client_ptr();

	string err_msg = ": 402 " + client->get_nickname() + " " + msg[1] + " :No such server";
	msg.append_out(err_msg);
}

void	CommandManager::err_nosuchchannel( Message& msg )
{
	Client*	client = msg.get_client_ptr();

	msg.append_out(": 403 " + client->get_nickname() + " " + msg[1] + " :No such channel");
}

void	CommandManager::err_noorigin( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	msg.append_out(": 409 " + client->get_nickname() + " :No origin specified");
}
/* ":No nickname given"*/
void	CommandManager::err_nonicknamegiven( Message& msg)
{
	Client*	client = msg.get_client_ptr();

	msg.append_out(": 431 " + client->get_nickname() + " :No nickname given");
}

/*"<client> <nick> :Erroneus nickname"*/
void	CommandManager::err_erroneusnickname( Message& msg)
{
	Client*	client = msg.get_client_ptr();

	string err_msg = ": 432 " + client->get_nickname() + " " + msg[1] + " :Erroneus nickname";
	msg.append_out(err_msg);
}

void	CommandManager::err_nicknameinuse( Message& msg)
{
	Client*	client = msg.get_client_ptr();

	string err_msg = ": 433 " + client->get_nickname() + " " + msg[1] + " :Nickname is already in use";
	msg.append_out(err_msg);
}

void	CommandManager::err_notonchannel( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	msg.append_out(": 442 " + client->get_nickname() + " " + msg[1] + " :You're not on that channel");
}

void	CommandManager::err_useronchannel( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	msg.append_out(": 443 " + client->get_nickname() + " " + msg[1] + " " + msg[2] + " :is already on channel");
}

/*NUM REPLIES - Users message*/
void	CommandManager::err_needmoreparams( Message& msg )
{
	Client*	client = msg.get_client_ptr();

	string err_msg = ": 461 " + client->get_nickname() + " " + msg[0] + " :Not enough parameters";
	msg.append_out(err_msg);
}

void	CommandManager::err_alreadyregistered( Message& msg )
{
	Client&	client = *msg.get_client_ptr();

	string err_msg = ": 462 " + client.get_nickname() + " :You may not re-register";
	msg.append_out(err_msg);
}

void	CommandManager::err_passwdmismatch( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	string err_msg = ": 464 " + client->get_nickname() + " :Password incorrect";
	msg.append_out(err_msg);
}

void	CommandManager::err_keyset( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	msg.append_out(": 467 " + client->get_nickname() + " " + msg[1] + " :Channel key already set");
}

void	CommandManager::err_badchanmask( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	string err_msg = ": 476 " + client->get_nickname() + " " + msg[1] + " :Invalid channel name.";
	msg.append_out(err_msg);
}

void	CommandManager::err_noprivileges( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	string err_msg = ": 481 " + client->get_nickname() + " :Permission Denied- You're not an IRC operator";
	msg.append_out(err_msg);
}

void	CommandManager::err_chanoprivsneeded( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	string err_msg = ": 482 " + client->get_nickname() + " " + msg[1] + " :You're not channel operator";
	msg.append_out(err_msg);
}

void	CommandManager::err_cantkillserver( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	string err_msg = ": 483 " + client->get_nickname() + " :You can't kill a server";
	msg.append_out(err_msg);
}

void 	CommandManager::err_nooperhost( Message& msg )
{
	Client*	client = msg.get_client_ptr();
	msg.append_out(": 491 " + client->get_nickname() + " :Given name/host is currently unavailable as operator.");
}

} // namespace irc end bracket
