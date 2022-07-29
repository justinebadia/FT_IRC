/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:53:04 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/29 11:15:38 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string>

using std::string;

class Client;
class Channel;
class Message;

namespace irc {

typedef struct	pollfd			t_pollfd;
typedef struct	sockaddr_in6	t_sockaddr6;
typedef int		size_fd; // change this if we wanna use another data type

class Socket 
{
private:
	t_pollfd	_pollfd;
	t_sockaddr6	_address;
	string		buffin;
	string		buffout;
	bool		_opened;
	
	
public:
	/* Constructors */
	Socket( void );
	Socket( const Socket & other );
	Socket& operator=( const Socket& other );
	
	~Socket( void );
	

	/* Getters */
	const t_pollfd &	getPollFD ( void ) const;
	const t_sockaddr6 &	getSockAddr ( void ) const;
	const size_fd &		getFD ( void ) const;
	short				getREvents ( void ) const;
	short				getEvents ( void ) const;

	/* Setters */
	void	setEvents ( const short & fde );
	void	setPollFD ( const t_pollfd & pfd );
	void	setFD ( const size_fd & fd );

	/* Utils */
	bool	isEvent ( int event ) const ;
	bool	isOpened ( void ) const;


};

} // namespace bracket

#endif