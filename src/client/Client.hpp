/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/27 18:02:09 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <list>
#include "ip.hpp"

using std::map;
using std::string;
using std::list;

namespace irc {

class Client {

private:

	string	_nickname;
	string	_username;
	
	static map<string, Client> _client_map;

public:
	Client( void ); // default constructor
	Client( const Client& other ); // copy constructor
	Client&	operator=( const Client& other ); // copy operator overload
	~Client( void ); // destructor
	
	static list<Client> _client_list; 

	string	get_nickname( void ) const;
	string	get_username( void ) const;
};

std::ostream	&operator << ( std::ostream & o, const Client& obj );

} // namespace irc end scope

#endif

/* notes:
user status:
->pending (connected to the server, but doesn't have a nick yet);
->ban 
->kick
->
*/
