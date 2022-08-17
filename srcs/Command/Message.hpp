

#pragma once
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <string>

#include "Client.hpp"
#include "irc_define.hpp"
#include "typedef.hpp"

#define MSG_DELIMITER	" "

using std::string;

/*============================================================================*/
namespace irc {

class Message {

private:
	
	/*---------------PROHIBITED-CONSTRUCTORS--------------*/

	

	/*--------------------ATTRIBUTES---------------------*/
	
	// Message		_empty_object;								// call the default constructor to create an instance on the stack
	Client*		_client_ptr;
	string		_message_in;
	string		_message_out;
	string 		_mode_rpl;
	
public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/
	
	Message( void ) : _client_ptr( NULL ) {  };				// default constructor
	Message( Client* client_ptr );							// main constructor
	Message( Client* client, const string& buffin );
	Message( const Message& rhs );							// copy constructor
	Message& operator=( const Message& rhs );				// copy operator overload
	~Message( void );										// destructor
	

	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Message& rhs ) const;
	string	operator[]( int rhs );
	string	operator[]( size_t rhs );

	/*-----------------------GETTERS----------------------*/

	Client*			get_client_ptr( void ) const;
	const string&	get_message_in( void ) const;
	const string&	get_message_out( void ) const;
	string			get_substr_after( const string& symbol );
	int 			get_param_count( void );
	string			get_mode_rpl( void );

	/*-----------------------SETTERS----------------------*/

	void	set_client_ptr( Client* client_ptr );
	void	set_mode_rpl( string );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	void			append_in( const string& str );
	void			append_out( const string& str );
	void			clear_all( void );

	friend std::ostream&	operator<<( std::ostream& o, const Message& obj );
};

	/*----------------NON-MEMBER-FUNCTIONS----------------*/
	
	std::ostream&	operator<<( std::ostream& o, const Message& obj );

} // namespace irc end bracket

#endif
