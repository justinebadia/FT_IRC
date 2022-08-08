/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42quebec.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:46:41 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/08 13:10:04 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

using namespace irc;

/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Message::Message( Client* client ) : _client_ptr(client)	// main constructor
{}

Message::Message( const Message& rhs )						// copy constructor
{
	*this = rhs;
}

Message&	Message::operator=( const Message& rhs )		// copy operator overload
{
	_message_in		= rhs._message_in;
	_message_out	= rhs._message_out;
	_client_ptr		= rhs._client_ptr;

	return *this;
}

Message::~Message( void )									// destructor
{}


/*-------------OTHER-OPERATOR-OVERLOADS-------------*/

//where is bool	operator==( const Message& rhs )			// equal equal operator overload

string		Message::operator[]( int i )					// scope operator overload
{
	size_t start	= 0;
	size_t last		= 0;
	size_t next		= 0;
	size_t len		= _message_in.length();

	while (i >= 0)
	{   
		next = _message_in.find(MSG_DELIMITER, last);
		if (next == string::npos)
		{
			if (i > 0)
			{
				return string("");
			}
			next = _message_in.length();
		} 
		len = next - last;
		start = last;
		last = next + 1;
		i--;
	}
	return _message_in.substr(start, len);
}


/*-----------------------GETTERS----------------------*/

Client*			Message::get_client_ptr( void ) const { return _client_ptr; }
const string&	Message::get_message_in( void ) const { return _message_in; }
const string&	Message::get_message_out( void ) const { return _message_out; }


/*-----------------------SETTERS----------------------*/

void	Message::set_client_ptr( Client* client )
{
	_client_ptr = client;
}


/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

void	Message::append_out( const string& str )
{
	 _message_out.append(str);
}

void	Message::append_in( const string& str )
{
	 _message_in.append(str);
}

string	Message::find_realname( void )
{
	string input = get_message_in();
	size_t delim_pos = input.find(":", 0);
	string realname = input.substr(delim_pos, input.npos);

	return realname;
}

void Message::clear_all( void )
{
	_message_in.clear();
	_message_out.clear();
}


/*----------------NON-MEMBER-FUNCTIONS----------------*/

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

