/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <tshimoda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:39:27 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/12 20:20:07 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <regex>
#include <string>
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

namespace irc
{

int		error_log( const string& src, const string& msg, int error_code )
{
	cerr << "Error " << error_code << ": " << src << ": " << msg << endl;
	return error_code;
}

bool	validate_entry( string regex_format, string entry)
{
	regex	reg_cmp(regex_format);
	bool res = std::regex_match(entry, reg_cmp);
	return res;
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

