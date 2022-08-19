#include "CommandManager.hpp"
#include "Server.hpp"
#include "irc_define.hpp"
#include "numeric_replies.hpp"
#include "typedef.hpp"
#include "utils.hpp"
#include "Channel.hpp"


using namespace irc;
using std::cout;
using std::cerr;
using std::endl;



void 	CommandManager::cmd_who( Message& msg )
{
	Client* client = msg.get_client_ptr(); 

	if (msg.get_param_count() >= 1)
	{
		if (msg[1][0] == '&' || msg[1][0] == '#')
		{
			Channel* channel = _database->get_channel(msg[1]);
			if (channel)
			{
				if (channel->is_member(client))
				{
					run_reply(RPL_WHOREPLY, msg);
					run_reply(RPL_ENDOFWHO, msg);
					return ;	
				}
			}
		}
		else
		{
			Client* target = _database->get_client(msg[1]);
			if (target)
			{
				run_reply(RPL_WHOREPLY, msg);
				run_reply(RPL_ENDOFWHO, msg);
				return;	
			}
		}
	}
	return ;
}
