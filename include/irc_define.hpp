/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_define.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:21:32 by tshimoda          #+#    #+#             */
/*   Updated: 2022/07/29 11:10:17 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace irc {

#define SUCCESS 0
#define FAIL -1

// MAXIMUMS
#define MAX_IN				512 //bytes
#define MAX_OUT				512 //bytes
#define MAX_QUEUE			5
#define MAX_CLIENTS			20
#define MAX_PENDING			10
#define MAX_CHANNELS		20
#define MAX_CLIENT_PER_CHAN	10
#define MAX_CHAN_PER_CLIENT	2

//Errors
#define	ERR_ALLOC	1

}
// namespace bracket