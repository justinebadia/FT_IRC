/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 17:10:17 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>
#include <map>
#include <list>

#include "irc_define.hpp"
#include "typedef.hpp"
#include "Client.hpp"

#define MSG_DELIMITER	" "

using std::string;

namespace irc {

class Message {

private:	
	Client*		_client_ptr;
	string		_message_in;
	string		_message_out;
	
	Message( void ) : _client_ptr( NULL ) {  };
public:
	Message( Client* client_ptr );
	Message( const Message& rhs );
	Message&	operator=( const Message& rhs );
	~Message( void );
	
	bool	operator==( const Message& rhs) const;
	string	operator[]( int rhs);

	/* getters */
	Client*			get_client_ptr( void ) const;
	const string&	get_message_in( void ) const;
	const string&	get_message_out( void ) const;

	/* setters */
	void	set_client_ptr( Client* client_ptr );

	/* utils */
	void	append_in( const string& str );
	void	append_out( const string& str );

	friend std::ostream&	operator<<( std::ostream& o, const Message& obj );
};

std::ostream&	operator<<( std::ostream& o, const Message& obj );

} // namespace irc end scope

#endif
