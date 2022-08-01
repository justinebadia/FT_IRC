/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_define.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:21:32 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/31 14:57:05 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef IRC_DEFINE_HPP
#define IRC_DEFINE_HPP

namespace irc {

#define SUCCESS 0
#define FAIL -1

// MAXIMUMS
#define MAX_IN				512 //bytes
#define MAX_OUT				512 //bytes
#define MAX_CLIENTS			20	// Amount of connections
#define MAX_PENDING			10 	// Amount of clients in registation phase 
#define MAX_CHANNELS		20	// Amount of created channels
#define MAX_CLIENT_PER_CHAN	10	// Channel capacity
#define MAX_CHAN_PER_CLIENT 5

// BUFFERS
#define BUFFIN	0
#define BUFFOUT	1

//Errors
#define	ERR_ALLOC	1

} // namespace end bracket

#endif
