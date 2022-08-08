

#include <algorithm>

#include "Database.hpp" 
#include "Client.hpp"
#include "Channel.hpp"
#include "typedef.hpp"

using std::cout;
using std::cerr;
using std::endl;

namespace irc
{

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
}	


Database::~Database( void )										// default destructor
{
	_client_list.clear();
	_channel_list.clear();
	_channel_clients_list_map.clear();
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


t_client_ptr_list	Database::get_clients_in_channel( const string& chan_name )
{
	t_client_ptr_list				clients_in_channel_list;
	t_channel_clients_map::iterator it;
	t_client_list::iterator			it_client;

	it = _channel_clients_list_map.find(chan_name);
	if (it != _channel_clients_list_map.end())
		return (*it).second;
	return t_client_ptr_list();
}

Channel*	Database::get_channel( const string& chan_name )
{
	t_channel_list::iterator it;

	for (it = _channel_list.begin(); it != _channel_list.end(); it++)
	{
		if ((*it).get_channel_name() == chan_name)
			return &(*it);
	}
	return NULL;
}

/*--------------------------OTHER-MEMBER-FUNCTIONS---------------------------*/

void	Database::init_Database( void )
{

}

void	Database::add_client( const Client& client )
{
	_client_list.push_back(client);
}

int		Database::add_client_to_channel( Client* client, string chan_name )
{
	t_channel_clients_map::iterator	it;
	t_client_ptr_list*				client_list;

	it = _channel_clients_list_map.find(chan_name);
	if (it != _channel_clients_list_map.end())
	{
		client_list = &(*it).second;
		if (std::find(client_list->begin(), client_list->end(), client) == client_list->end())
			client_list->push_front(client);
		return 0;
	}
	return -1;
}


void	Database::remove_client( const string& nickname )
{
	Client* c;

	c = get_client(nickname);
	if (c == NULL)
		return;
	_client_list.remove(*c);
	remove_client_from_all_channels(c->get_nickname());//WARNING: nee//WARNING: need a more complete removal (banlist, links with channels, etc.)
}

void	Database::remove_client( const int& fd )
{
	Client* c;

	c = get_client(fd);
	if (c == NULL)
		return;
	_client_list.remove(*c);
	remove_client_from_all_channels(c->get_nickname());//WARNING: need a more complete removal (banlist, links with channels, etc.)
}

void	Database::remove_channel( const string& chan_name)
{
	remove_all_clients_from_channel(chan_name);
	// WARNING : need to do more stuff when we'll be further
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

};
