/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/27 14:18:46 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>

class Client;
class Channel;
class Message;

class Server {

private:

	int											server_socket;
	unsigned int								port;
	std::string									password;
	std::list<std::pair<Client* , Channel* > >	database;
	//std::vector<OPER> ???
//	std::map<int, void (Message::*reply_function)( int reply )> reply_map;
	
	
	
public:
	Server( void );
	~Server( void );
	Server( const Server& other );
	Server& operator=( const Server& other );

};

#endif