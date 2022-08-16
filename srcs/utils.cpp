/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:39:27 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/15 12:08:04 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <regex>
#include <string>
#include <list>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "utils.hpp"


using std::string;
using std::cerr;
using std::endl;
using std::regex;
using std::list;

namespace irc
{

bool	validate_entry( string regex_format, string entry)
{
	regex	reg_cmp(regex_format);
	bool res = std::regex_match(entry, reg_cmp);
	return res;
}

bool	compare_to_mask_list( list<string>* mask_list, const string& str )
{
	list<string>::iterator	it;

	for(it = mask_list->begin(); it != mask_list->end(); it++)
	{
		if (compare_to_mask(*it, str))
			return true; 
	}
	return false;
}

bool	compare_to_mask( const string& mask, const string& str )
{
	size_t	mask_i = 0;
	size_t	str_i = 0;
	size_t	next_wild;
	size_t	find_mask;
	size_t	sublen;

	if (mask[mask_i] != '*' && mask[mask_i] != str[str_i])
		return false;
	if (mask[mask.length() - 1] != '*' && mask[mask.length() - 1] != str[str.length() - 1])
		return false;

	while (mask[mask_i])
	{
		while (mask[mask_i] == '*')
			mask_i++;

		if (!mask[mask_i])
			return true;

		// Get the index of the next (*)
		next_wild = mask.find("*", mask_i);
		if (next_wild == string::npos)
			next_wild = mask.length() - 1;
		sublen = next_wild - mask_i;

		// Attempt to find the mask substr
		find_mask = str.find(mask.substr(mask_i, sublen), str_i);
		if (find_mask == string::npos)
			return false;
		
		mask_i += sublen + 1;
		str_i = find_mask + sublen;
	}
	return true;
}

const string grab_ip_address( void )
{
	char hostname[128];
	char ip[16];
	struct hostent* host;
	struct sockaddr_in sock_addr;

	gethostname(hostname, sizeof(hostname));

	host = gethostbyname(hostname);

	for (int i = 0; host->h_addr_list[i]; i++) 
	{
		sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);
		inet_ntop(AF_INET, &sock_addr.sin_addr, ip, sizeof(ip));
	}
	return (string(ip));
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

