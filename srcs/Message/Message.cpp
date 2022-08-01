/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 16:37:27 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 14:36:38 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 10:24:55 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

using namespace irc;

Message::Message( Client* client ) : _client_ptr(client) {  }

Message::Message( const Message& rhs )
{
	*this = rhs;
}

Message&	Message::operator=( const Message& rhs )
{
	_message_in		= rhs._message_in;
	_message_out	= rhs._message_out;
	_client_ptr		= rhs._client_ptr;
	return *this;
}

string		Message::operator[]( int i )
{
	size_t start	= 0;
	size_t last		= 0;
	size_t next		= 0;
	size_t len		= _message_in.length();

	while (i-- >= 0)
	{   
		next = _message_in.find(MSG_DELIMITER, last);
		if (next == string::npos)
		{
			if (i > 0)
				return "";
			next = _message_in.length();
		} 
		start = last;
		len = next - last;
		last = next + 1;
	}
	return _message_in.substr(start, len);
}

Message::~Message( void ) {  }

/* getters */
Client*			Message::get_client_ptr ( void ) const { return _client_ptr; }
const string&	Message::get_message_in ( void ) const { return _message_in; }
const string&	Message::get_message_out ( void ) const { return _message_out; }

/* setters */
void	Message::set_client_ptr ( Client* client )
{
	_client_ptr = client;
}

/* utils */
void	Message::append_out ( const string& str )
{
	 _message_out.append(str);
}

void	Message::append_in ( const string& str )
{
	 _message_in.append(str);
}

std::ostream&	irc::operator<<( std::ostream& o, const Message& obj )
{
	o << "-- MESSAGE CONTENT --" << std::endl;
	o << "Client fd and nickname : "; 
	if (obj._client_ptr == NULL)
		o << "NULL";
	else
		o << obj._client_ptr->get_fd() << " " << obj._client_ptr->get_nickname();
	o << std::endl << "message_in : " << obj._message_in << std::endl;
	o << "message_out : " << obj._message_out;
	return o;
}
