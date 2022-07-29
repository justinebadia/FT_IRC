/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/29 10:14:34 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

using namespace irc;

	Socket::Socket( void ) : _opened(false) {  }
	
	Socket::Socket( const Socket & other )
	{
		*this = other;
	};
	Socket& Socket::operator=( const Socket& other )
	{
		_pollfd = other.getPollFD();
		_address = other.getSockAddr();
		_opened = other.isOpened();
	};

	Socket::~Socket( void ) {  }

	const t_pollfd &	Socket::getPollFD ( void ) const 
	{
		return _pollfd;
	};

	const t_sockaddr6 &	Socket::getSockAddr ( void ) const
	{
		return _address;
	};

	const size_fd &		Socket::getFD ( void ) const
	{
		return _pollfd.fd;
	}

	short	Socket::getEvents ( void ) const
	{
		return (_pollfd.events);
	}

	short	Socket::getREvents ( void ) const
	{
		return (_pollfd.revents);
	}

	void	Socket::setFD ( const t_pollfd & pfd )
	{
		_pollfd = pfd;
	}

	void	Socket::setEvents ( const short & fde )
	{
		_pollfd.events = fde;
	}


	bool	Socket::isEvent ( int event ) const
	{
		return (getREvents() & event);
	}

	bool	Socket::isOpened ( void ) const
	{
		return (_opened);
	}

