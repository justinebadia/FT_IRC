/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 16:35:14 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 17:11:12 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "typedef.hpp"

using namespace irc;

typedef struct s_socket
{
	t_pollfd	pollfd;
	t_addr6		addr;
}	t_socket;
