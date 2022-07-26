#include "Client.hpp"

Client::Client()
{

}

Client::Client( Client const &obj )
{

}

Client::~Client()
{
}

Client &	Client::operator=( Client const & obj )
{
	std::cout << "Copy assignment operator called" << std::endl;
	return *this;
}

void		Client::setVar( type var)
{
	_var= var;
}

type		Client::getVar( void ) const
{
	return _var;
}

std::ostream &	operator<<( std::ostream & o, Client const & obj )
{
	return o;
}