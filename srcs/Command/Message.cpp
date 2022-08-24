
#include "Message.hpp"

#include <string>
#include <iostream>

#include "Client.hpp"

using namespace irc;

/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Message::Message( Client* client ) : _client_ptr(client)	// main constructor
{}

Message::Message( Client* client, const string& buffin ) : _client_ptr(client), _message_in(buffin)	// main constructor
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

string		Message::operator[]( int i )					// scope operator overload
{
	size_t start	= 0;
	size_t last		= 0;
	size_t next		= 0;
	size_t len		= _message_in.length();
	size_t colon 	= _message_in.find(" :", 0);

	while (i >= 0)
	{   
		next = _message_in.find(MSG_DELIMITER, last);
		if (next == string::npos || (next > colon && colon != string::npos))
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

string		Message::operator[]( size_t i )					// scope operator overload
{
	return operator[](static_cast<int>(i));
}


/*-----------------------GETTERS----------------------*/

Client*			Message::get_client_ptr( void ) const { return _client_ptr; }
const string&	Message::get_message_in( void ) const { return _message_in; }
const string&	Message::get_message_out( void ) const { return _message_out; }
string 			Message::get_mode_rpl( void ) { return _mode_rpl; }
string 			Message::get_special_output( void ) { return _special_output; }


string		Message::get_substr_after( const string& symbol )
{
	size_t pos;
	string symb = " " + symbol;
 
	pos = _message_in.find(symb, 1);
	if (pos == string::npos)
		return string("");
	return _message_in.substr(pos + symb.length());
}



int		Message::get_param_count( void )
{
	int i = 0;

	while(!(*this)[i].empty())
		i++;
	return (i - 1);
}

/*-----------------------SETTERS----------------------*/

void	Message::set_client_ptr( Client* client )
{
	_client_ptr = client;
}

void Message::set_mode_rpl(const string& mode_reply) { _mode_rpl = mode_reply;}
void Message::set_special_output(const string& output_reply) { _special_output = output_reply;}


/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

void	Message::append_out( const string& str )
{
	 _message_out.append(str);
}

void	Message::append_in( const string& str )
{
	 _message_in.append(str);
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

