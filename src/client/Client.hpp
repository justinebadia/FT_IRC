/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/27 18:02:09 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <list>
#include "ip.hpp"

using std::map;
using std::string;
using std::list;

class Client
{
private:
	static map<string, Client> _clients;


public:
	Client();
	Client( const Client& obj );
	~Client();

	Client&	operator=( const Client& obj );
	static list<Client> _client_list; 
};

std::ostream	&operator << ( std::ostream & o, const Client& obj );

#endif
