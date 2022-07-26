/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:11:05 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/26 16:21:09 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include "ip.hpp"

using std::map;
using std::string;

class Client
{
private:
	static map<string, Client> _clients;


public:
	Client();
	Client( Client const &obj );
	~Client();

	Client &	operator=( Client const &obj );
};

std::ostream	&operator << ( std::ostream & o, Client const & obj );

#endif
