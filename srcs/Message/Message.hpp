/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:31:25 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 09:19:45 by tshimoda         ###   ########.fr       */
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

/*============================================================================*/
namespace irc {

class Message {

private:

	Message( void ) : _client_ptr( NULL ) {  };				// default constructor [PRIVATE]
	
	/*--------------------ATTRIBUTES---------------------*/
	
	Message		_empty_object;								// call the default constructor to create an instance on the stack
	Client*		_client_ptr;
	string		_message_in;
	string		_message_out;
	
public:

	Message( Client* client_ptr );							// main constructor
	Message( const Message& rhs );							// copy constructor
	Message& operator=( const Message& rhs );				// copy operator overload
	~Message( void );										// destructor
	

	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Message& rhs) const;
	string	operator[]( int rhs);

	/*-----------------------GETTERS----------------------*/

	Client*			get_client_ptr( void ) const;
	const string&	get_message_in( void ) const;
	const string&	get_message_out( void ) const;

	/*-----------------------SETTERS----------------------*/

	void	set_client_ptr( Client* client_ptr );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void	append_in( const string& str );
	void	append_out( const string& str );

	friend std::ostream&	operator<<( std::ostream& o, const Message& obj );
};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/
	
	std::ostream&	operator<<( std::ostream& o, const Message& obj );

} // namespace irc end scope

#endif
