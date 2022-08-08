/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 18:23:58 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Message.hpp"

namespace irc
{

void	cmd_nick( Message& msg );
void	cmd_user( Message& msg );
void    cmd_whois( Message & msg );

}

#endif
