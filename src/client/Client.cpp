#include "Client.hpp"

Client::Client( void ) // default constructor
{}

Client::Client( Client const &obj ) // copy constructor
{}

Client::~Client( void ) // destructor
{}

Client&	Client::operator=( Client const & obj )
{
	std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

void	Client::setVar( type var)
{
	_var= var;
}

type		Client::getVar( void ) const
{
	return _var;
}

string	get_nickname( void ) const
{
	return this->_nickname;
}

string	get_username( void ) const
{
	return this->_username;
}

std::ostream &	operator<<( std::ostream & o, Client const & obj )
{
	return o;
}
