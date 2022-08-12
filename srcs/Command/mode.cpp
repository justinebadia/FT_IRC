#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "Channel.hpp"

using namespace irc;
using std::cout;
using std::cerr;
using std::endl;


int parse_cmd_mode( Message& msg ) // si erreur -42
{
	Client*		client	= msg.get_client_ptr();

	if (msg[1][0] == '#' || msg[1][0] == '&')
	{
		if (msg.get_param_count() < 3)
		{
			irc::CommandManager::run_reply(ERR_NEEDMOREPARAMS, msg);
			return ;
		}
		return 1; // 1 = mode channel
	}
	else
		return 0; // mode user
}



void CommandManager::cmd_mode( Message& msg ) //attention les yeux - faire une fonction qui parse la commande
{
	Client*		client	= msg.get_client_ptr();
	t_channel_ptr_list chan_list;

if (parse_cmd_mode(msg) == 1)
{	
	Channel *channel = _database->get_channel(msg[1]);
	if (!channel)
	{
		run_reply(ERR_NOSUCHCHANNEL, err_nosuchchannel);
		return ;
	}
	if (!(channel->is_chanop(client)) || !(channel->is_owner(client)))
	{
		run_reply( ERR_CHANOPRIVSNEEDED, err_chanoprivsneeded);
		return ;
	}
	// if (channel->)
	// {
	// 	run_reply(ERR_NOTONCHANNEL , err_notonchannel);
	// 	return ;
	// }
	

}
// 	if (!msg[1].empty() && msg[1][0] == '#')
// 	{
// 		Channel *channel = _database->get_channel(msg[1]);
// 		if (channel)
// 		{
// 			if(!msg[2].empty() && msg[2][0] == '+')
// 			{
// 					if (!msg[3].empty() && (channel->is_chanop(client) || channel->is_owner(client)))
// 					{
// 						Client* target = _database->get_client(msg[4]);
// 						if(!msg[4].empty() && channel->is_member(target))
// 						{
// 							if (msg[2][1] == 'o')
// 								channel->set_permission(target, Channel::e_permission::CHANOP);
							
// 						}
// 					}
// 				}
// 			}	
// 		}
	}