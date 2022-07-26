/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ip.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 15:14:51 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/26 15:19:44 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef IP_HPP
#define IP_HPP

#include <string>

namespace irc { 

typedef struct s_ip
{
	std::string ip;
	bool		v6 = false;

	s_ip();
	s_ip( std::string ip, bool v6 ) 
	{ 
		this->ip = ip;
		this->v6 = v6;
	}
	
}	t_ip;

};

#endif
