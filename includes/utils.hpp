/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:25:24 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/24 14:59:22 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <regex>
#include <list>
#include "typedef.hpp"

using std::string;
using std::list;

namespace irc
{

bool	        validate_with_regex( string regex_format, string entry );
bool			compare_to_mask( const string& mask, const string& str );
bool			compare_to_mask_list( list<string>* mask_list, const string& str );
string    		convert_ip_address( const string& ip );
// const string    grab_ip_address( void );

}

#endif
