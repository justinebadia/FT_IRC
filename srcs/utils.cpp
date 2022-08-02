/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:39:27 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 09:36:16 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <regex>
#include <string>
#include "utils.hpp"

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::regex;

namespace irc
{

int		error_log( const string & src,const string & msg, int error_code )
{
	cerr << "Error " << error_code << " : " << msg << endl;
	return error_code;
}

bool	validate_entry( string regex_format, string entry)
{
	regex	reg_cmp(regex_format);
	bool res = std::regex_match(entry, reg_cmp);
	return res;
}

// template < class Cont, typename value_type>
// Allocated on the stack; works with most STL containers
// value_type*	convert_stl_to_array ( const Cont & cont )
// {
// 	value_type array[cont.size()];
// 	try
// 	{
//     	std::copy(cont.begin(), cont.end(), array);
// 	}
// 	catch (std::exception & e)
// 	{
// 		string msg = e.what();
// 		msg.append(" Returning NULL");
// 		error_log("convert_stl_to_array", msg , errno );
// 	}
// 	return array;
// }

} // namespace irc end bracket

