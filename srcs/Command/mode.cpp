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
		if (msg.get_param_count() < 1)
		{
			irc::CommandManager::run_reply(ERR_NEEDMOREPARAMS, msg);
			return -1;
		}
		return 1; // 1 = mode channel
	}
	else
		return 0; // mode user
}


/*quand a t'on cette err : ERR_UNKNOWNMODE ? 

        RPL_CHANNELMODEIS a faire dans chaque case
        ERR_NOSUCHNICK 
        ERR_KEYSET à faire dans le case du k
        RPL_BANLIST                     RPL_ENDOFBANLIST a mettre dans le case b
        ERR_UNKNOWNMODE                 
 */
void CommandManager::cmd_mode( Message& msg ) //attention les yeux - faire une fonction qui parse la commande
{
	Client*		client	= msg.get_client_ptr();
	t_channel_ptr_list chan_list;

	if (parse_cmd_mode(msg) == 1)
	{	
		Channel *channel = _database->get_channel(msg[1]);
		if (!channel)
		{
			run_reply(ERR_NOSUCHCHANNEL, msg);
			return ;
		}
		if (!(channel->is_chanop(client)) && !(channel->is_owner(client)))
		{
			run_reply(ERR_CHANOPRIVSNEEDED, msg);
			return ;
		}
		if (!channel->is_member(client))
		{
			run_reply(ERR_NOTONCHANNEL, msg);
			return ;
		}
		if (channel->parse_modes(msg[2]) > 0) // vérifier si des flags sont actifs
		{
			if (msg.get_param_count() < 3) // commande possible sans nickname +i +t +b
			{
				switch (channel->get_mode_flags())
				{
					case Channel::FLAG_I: //modifier join
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						break;
					}
					case Channel::FLAG_T:
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						break;
					}
					case Channel::FLAG_B: 
					{
						run_reply(RPL_BANLIST, msg);
						run_reply(RPL_ENDOFBANLIST, msg);	
						break;
					}
					case (Channel::FLAG_I + Channel::FLAG_T): 
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						break;
					}
				}
			}
			else //command ou nickname est obligatoire +o +b +k
			{
				Client* target	= _database->get_client(msg[3]);// si client n'existe pas, on le skip
				string password = ""; // donc s'il y a k, le param va sur le k
				// if (!channel->is_member(target)) QUEL REPLY SI NON MEMBRE ? PAS IMPORTANT - faire plus de test
				// {

				// }
				switch (channel->get_mode_flags())
				{
					case Channel::FLAG_O: 
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						if (channel->is_member(target))
							channel->set_permission(target, CHANOP);
						break;
					}
					case Channel::FLAG_B: // modifier cmd_join et cmd_privmsg
					{
						// if (channel->get_permission(target) == Channel::REGULAR)
						//	ajouter la target à la banlist, pas besoin qu'il fasse parti du channel
					}
					case Channel::FLAG_K: 
					{
						run_reply(RPL_CHANNELMODEIS, msg); // aller intégrer mode +k à cmd_join
						break;
					}
					case (Channel::FLAG_K + Channel::FLAG_O): // je dois avoir 2 param pour chaque
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						break;
					}
					case (Channel::FLAG_K + Channel::FLAG_B): // je dois avoir 2 param pour chaque
					{
						run_reply(RPL_CHANNELMODEIS, msg);
						break;
					}
				}
			}
	}
	else if (parse_cmd_mode(msg) == 0)// MODE user case
	{
		return ;
	}
}
}