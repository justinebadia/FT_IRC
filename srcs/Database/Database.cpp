

#include <algorithm>
#include <sys/syslimits.h>

#include "Server.hpp"
#include "Database.hpp" 
#include "Client.hpp"
#include "Channel.hpp"
#include "typedef.hpp"
#include "color.hpp"

using std::cout;
using std::cerr;
using std::endl;
namespace irc {

/*--------------------------CONSTRUCTORS-&-DESTRUCTOR--------------------------*/

Database::Database( void ) // default constructor [PRIVATE]
{ 
	init_Database();
}	

Database::Database( const Database& other ) // copy constructor [PRIVATE]
{
	*this = other;
}

Database&	Database::operator=( const Database& other ) // copy operator overload [PRIVATE]
{
	_client_list = t_client_list(other._client_list);
	_channel_list = t_channel_list(other._channel_list);
	_invite_coupon_list = t_invite_coupon_list(other._invite_coupon_list);
	return *this;
}	


Database::~Database( void )										// default destructor
{
	_client_list.clear();
	_channel_list.clear();
	_invite_coupon_list.clear();
//	_channel_clients_list_map.clear();
	// WARNING : add any other container to clear

}


/*---------------------------------GETTERS-----------------------------------*/

// Database&	Database::get_Database( void ) // singleton
// {
// 	static Database singleton(void); // static singleton declared on the stack, call the main constructor
// 	return singleton;
// }

// [Client related getters]
const t_client_list&	Database::get_client_list( void ) { return _client_list; }

const t_client_ptr_list	Database::get_client_ptr_list( void ) 
{ 
	t_client_ptr_list		client_ptr_list;
	t_client_list::iterator	it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
		client_ptr_list.push_back(&(*it));
	return client_ptr_list;
}

size_t					Database::get_client_count( void ) { return _client_list.size(); }

Client*	Database::get_client( const int& fd )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_pollfd().fd == fd)
			return &(*it);
	}
	return NULL;
}

Client*	Database::get_client( const string& nickname )
{
	t_client_list::iterator it;

	for (it = _client_list.begin(); it != _client_list.end(); it++)
	{
		if ((*it).get_nickname() == nickname)
			return &(*it);
	}
	return NULL;
}


t_client_ptr_list	Database::get_clients_in_channel( const string& chan_name ) // TO UPDATE
{

	t_client_ptr_list				client_list_in_channel;
	Channel* channel = get_channel(chan_name);

	if (channel)
	{
		client_list_in_channel = channel->get_clients_any_permissions();
		return client_list_in_channel;
	}
	
	/*
	t_client_ptr_list				clients_in_channel_list;
	t_channel_clients_map::iterator it;
	t_client_list::iterator			it_client;

	it = _channel_clients_list_map.find(chan_name);
	if (it != _channel_clients_list_map.end())
		return (*it).second;
	*/
	return t_client_ptr_list();	// return une liste vide
}



t_client_ptr_list	Database::get_clients_in_channel( Channel* channel ) // TO UPDATE
{
	t_client_ptr_list				client_list_in_channel;

	if (channel && (is_channel_listed(channel->get_name()) == true))
			{
			client_list_in_channel = channel->get_clients_any_permissions();
			return client_list_in_channel;
			}
//	t_channel_clients_map::iterator it;
//	t_client_list::iterator			it_client;
	/*
	if (channel)
	{
		it = _channel_clients_list_map.find(channel->get_name());
		if (it != _channel_clients_list_map.end())
			return (*it).second;
	}
	*/
	return t_client_ptr_list();	// return une liste vide
}


Channel*	Database::get_channel( const string& chan_name )
{
	t_channel_list::iterator it;

	for (it = _channel_list.begin(); it != _channel_list.end(); it++)
	{
		if ((*it).get_name() == chan_name)
			return &(*it);
	}
	return NULL;
}

size_t		Database::get_channel_count( void ) { return _channel_list.size(); }


t_channel_ptr_list	Database::get_channel_list_of_client( Client *client ) // LIST DE CHANNELS WHERE THE CLIENT IS                  	                              
{
//	t_channel_clients_map::iterator it;
		t_channel_ptr_list list_of_client_chan;
		Channel *channel;

//	for(it =_channel_clients_list_map.begin(); it != _channel_clients_list_map.end(); it++)
	
	t_channel_list::iterator it = _channel_list.begin();
	t_channel_list::iterator ite = _channel_list.end();

	for (; it != ite; it++)
	{
		channel = &(*it);
		//Server::log("get_channel_list_of_client: Checking if client is in channel <" + channel->get_name() + ">...");
		//if (channel && std::find((*it).second.begin(), (*it).second.end(), client) != (*it).second.end())
		if (channel && channel->is_member(client))
		{
			//cout << "The!" << endl;
			list_of_client_chan.push_back(channel);
		}
	}
	return (list_of_client_chan);
}

t_client_ptr_list		Database::get_channel_in_common_recipient_list( Client* client )
{
	t_client_ptr_list	recipient_list;
	t_channel_ptr_list	joined_channel_list;

	joined_channel_list = get_channel_list_of_client(client);

	t_channel_ptr_list::iterator it = joined_channel_list.begin();
	t_channel_ptr_list::iterator ite = joined_channel_list.end();

	for (; it != ite; it++)
	{
		Channel* channel = (*it);

		t_channel_memberlist memberlist = channel->get_memberlist();
		t_channel_memberlist::iterator mit = memberlist.begin();
		t_channel_memberlist::iterator mite = memberlist.end();

		for (; mit != mite; mit++)
		{
			recipient_list.push_back((*mit).first);
		} 
	}
	recipient_list.sort();
	recipient_list.unique();
	
	return recipient_list;
}

t_invite_coupon_list	Database::get_invite_coupon_list( void ) { return _invite_coupon_list; }



/*--------------------------OTHER-MEMBER-FUNCTIONS---------------------------*/

void	Database::init_Database( void )
{

}

int	Database::add_client_list( const Client& client )
{
	Server::log("in Database add_client_list()");
	if (is_client_listed(client) == false)
	{
		_client_list.push_back(client);
		return SUCCESS;
	}
	return FAIL;	//client already in the client_list
}

int	Database::add_channel_list( const Channel& channel )
{
	Server::log("in Database add channel_list()");
	if (is_channel_listed(channel) == false)
	{
		_channel_list.push_back(channel);
		//_channel_clients_list_map.insert(std::make_pair(channel.get_name(), t_client_ptr_list()));
		return SUCCESS;
	}
	return FAIL; //channel already in the channel_list
}

bool	Database::is_client_listed( const Client& client )
{
	t_client_list::iterator it = _client_list.begin();
	t_client_list::iterator ite = _client_list.end();

	for (; it != ite; it++)
	{
		if ((*it).get_fd() == client.get_fd())
			return true;
	}
	return false;
}

bool	Database::is_client_listed( const int& fd )
{
	t_client_list::iterator it = _client_list.begin();
	t_client_list::iterator ite = _client_list.end();

	for (; it != ite; it++)
	{
		if ((*it).get_fd() == fd)
			return true;
	}
	return false;
}

bool	Database::is_client_listed( const string& nickname )
{
	t_client_list::iterator it = _client_list.begin();
	t_client_list::iterator ite = _client_list.end();

	for (; it != ite; it++)
	{
		if ((*it).get_nickname() == nickname)
			return true;
	}
	return false;
}

bool	Database::is_channel_listed( const Channel& channel )
{
	t_channel_list::iterator it = _channel_list.begin();
	t_channel_list::iterator ite = _channel_list.end();

	for (; it != ite; it++)
	{
		if ((*it).get_name() == channel.get_name())
			return true;
	}
	return false;
}

bool	Database::is_channel_listed( const string& chan_name )
{
	t_channel_list::iterator it = _channel_list.begin();
	t_channel_list::iterator ite = _channel_list.end();

	for (; it != ite; it++)
	{
		if ((*it).get_name() == chan_name)
			return true;
	}
	return false;
}

bool	Database::is_channel_empty( Channel* channel )
{
	if (is_channel_listed(channel->get_name()) == false)
			return true;
	if (channel->is_empty() == true)
		return true;
	else
		return false;

// OLD VERSION with map
/*	t_channel_clients_map::iterator	it;

	if (channel)
	{
		it = _channel_clients_list_map.find(channel->get_name());
		if (it != _channel_clients_list_map.end())
		{
			if (!(*it).second.empty())
				return false;
		}
	}
	return true;
*/
}



void	Database::delete_client_from_all_lists( Client* client )
{
	t_channel_list::iterator it = _channel_list.begin();
	t_channel_list::iterator ite = _channel_list.end();

	for (; it != ite; it++) // for loop to delete the client from every channel_memberlist
	{
		if ((*it).is_member(client) == true)
		{
			(*it).remove_member(client); // removing client from one channel_memberlist;
		}
	}
	if (is_client_listed(client->get_nickname()) == false)
	{
			return ; // client is not in the database
	}
	else
		_client_list.remove(*client);
}

void Database::delete_inactive_channels( void )
{	
	t_channel_ptr_list	channel_list;

	t_channel_ptr_list::iterator it = channel_list.begin();
	t_channel_ptr_list::iterator ite = channel_list.end();

	for (; it != ite; it++)
	{
		if ((*it)->is_empty() == true)
		{
			_channel_list.remove(*(*it)); // deleting one inactive channel
		}
	}
}

void Database::print_client_list( void )
{
	t_client_list::iterator it = _client_list.begin();
	t_client_list::iterator ite = _client_list.end();

	cout << GREEN << "-----------------------------" << RESET << endl;
	cout << GREEN << "Database::print_client_list()" << RESET << endl;
	for (; it != ite; it++)
	{
		cout << (*it).get_nickname() << endl;
	}
	cout << GREEN << "-----------------------------" << RESET << endl;
}

void Database::print_channel_list( void )
{
	t_channel_list::iterator it = _channel_list.begin();
	t_channel_list::iterator ite = _channel_list.end();

	cout << BLUE << "------------------------------" << RESET << endl;
	cout << BLUE << "Database::print_channel_list()" << RESET << endl;
	for (; it != ite; it++)
	{
		cout << (*it).get_name() << endl;
	}
	cout << BLUE << "------------------------------" << RESET << endl;
}

void Database::print_invite_coupon_list( void )
{
	t_invite_coupon_list::iterator it = _invite_coupon_list.begin();
	t_invite_coupon_list::iterator ite = _invite_coupon_list.end();

	
	cout << YELLOW << "------------------------------------" << RESET << endl;
	cout << YELLOW << "Database::print_invite_coupon_list()" << RESET << endl;
	for (; it != ite; it++)
	{
		cout << "Invite coupon for "<< (*it).first->get_nickname() << " to join channel " <<	(*it).second->get_name() << endl;
	}	
	cout << YELLOW << "------------------------------------" << RESET << endl;
}

/*
int		Database::add_client_to_channel( Client* client, const string& chan_name )
{
	return add_client_to_channel(client, get_channel(chan_name));
}
*/
/*
int		Database::add_client_to_channel( Client* client, Channel* channel )
{
	t_channel_clients_map::iterator	it;
	t_client_ptr_list*				client_list;

	if (!channel)
		return FAIL; // WARNING return not used. Might not be useful
	it = _channel_clients_list_map.find(channel->get_name());
	if (it != _channel_clients_list_map.end())
	{
		client_list = &(*it).second;
		if (std::find(client_list->begin(), client_list->end(), client) == client_list->end())
		{
			client_list->push_back(client);
		}
		return SUCCESS;
	}
	return FAIL;
}
*/


/*
void	Database::remove_client_list( const string& nickname )
{
	Client* c;

	c = get_client(nickname);
	if (c == NULL)
		return;
	_client_list.remove(*c);
	remove_client_from_all_channels(c->get_nickname());//WARNING: nee//WARNING: need a more complete removal (banlist, links with channels, etc.)
}

void	Database::remove_client_list( const int& fd )
{
	Client* c;

	c = get_client(fd);
	if (c == NULL)
		return;
	_client_list.remove(*c);
	// close(fd);
	remove_client_from_all_channels(c->get_nickname());//WARNING: need a more complete removal (banlist, links with channels, etc.)
}

void	Database::remove_client_list( Client* client )
{
	if (client == NULL)
		return;
	remove_client_from_all_channels(client->get_nickname());//WARNING: nee//WARNING: need a more complete removal (banlist, links with channels, etc.)
	_client_list.remove(*client);
}

void	Database::remove_channel_list( const string& chan_name )
{
	Channel* channel;

	if (is_channel_listed(chan_name) == true)
	{
		channel = get_channel(chan_name);
		if (channel)
		{
			Server::log("removed channel: " + chan_name);
			remove_all_clients_from_channel(chan_name);
			_channel_list.erase( std::find(_channel_list.begin(), _channel_list.end(), *channel) );
		}
	}
}

void	Database::remove_client_from_channel( const string& nickname, const string& chan_name )
{
	t_channel_clients_map::iterator	it;
	t_client_ptr_list*				client_list;
	t_client_ptr_list::iterator		it_client;

	it = _channel_clients_list_map.find(chan_name);
	if (it != _channel_clients_list_map.end())
	{
		client_list = &(*it).second;
		it_client = std::find(client_list->begin(), client_list->end(), get_client(nickname));
		if (it_client != client_list->end())
			client_list->erase(it_client);
	}
}



void	Database::remove_client_from_all_channels( const string& nickname )
{
	t_channel_clients_map::iterator	it;

	for (it = _channel_clients_list_map.begin(); it != _channel_clients_list_map.end(); it++)
	{
		remove_client_from_channel(nickname, (*it).first);
	}
}



void	Database::remove_all_clients_from_channel( const string& chan_name )
{
	_channel_clients_list_map.erase(chan_name);
}
*/

void	Database::clean_database( void )
{
	delete_inactive_channels();
}

void	Database::create_invite_coupon( Client* client, Channel* channel )
{
	// WARNING VERIFY IF INVITE_COUPON IS UNIQUE ???
	_invite_coupon_list.push_back(std::make_pair(client, channel));
}

void	Database::use_invite_coupon( Client* client, Channel* channel )
{
	t_invite_coupon_list::iterator it = _invite_coupon_list.begin();
	t_invite_coupon_list::iterator ite = _invite_coupon_list.end();

	for (; it != ite; it++)
	{
		if ((*it).first->get_nickname() == client->get_nickname() &&
			(*it).second->get_name() == channel->get_name())
		{
			_invite_coupon_list.erase(it);
			return;
		}
	}	
}

} // namespace irc end bracket
