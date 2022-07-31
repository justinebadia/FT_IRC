/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/31 13:04:15 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>

#define PORT 6667 // irc server port number

class Client;
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

	std::list<std::pair<Client* , Channel* > >	_database;
	
	//std::vector<OPER> ???
	//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	
	
public:
	Server( const unsigned int& port, const std::string password, bool exit ); // main constructor
	~Server( void ); // default destructor

	bool	get_exit_status( void );

};

} // namespace irc end scope

#endif
