/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/31 18:44:04 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <signal.h> // SIGINT == control-C
#include "typedef.hpp"
#include "irc_define.hpp"
#include "../Client/Client.hpp"

#define PORT 6667 // irc server port number
using std::string;

class Channel;
class Message;

namespace irc {

class Server {

private:

	Server( void ); // default constructor
	Server( const Server& other ); // copy constructor
	Server& operator=( const Server& other ); // copy operator overload 

	int			_server_socket;
	const unsigned int	_port;
	const std::string	_password;
	bool			_exit;

	t_client_list	_client_list;

	std::list<std::pair<Client* , Channel* > >	_database;
	
	//std::vector<OPER> ???
	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	
	
public:
	Server( const unsigned int& port, const std::string password, bool exit ); // main constructor
	~Server( void ); // default destructor

	void	add_client( const Client & client );
	void	remove_client( const string & nick );

	static Server&			get_server( const unsigned int& port = 0, const std::string password = "", bool exit = false ); // singleton
	bool					get_exit_status( void );
	const t_client_list&	get_client_list ( void );
	Client*					get_client ( int fd );
	Client*					get_client ( std::string nick );
	size_t					get_client_count ( void );
	t_pollfd*				get_pollfd_array ( void );

	void	set_exit_status( bool true_signal );
};

} // namespace irc end scope

#endif
