/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:39:27 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/29 11:05:17 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <list>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

int	error_log ( const string & src,const string & msg, int error_code )
{
	cerr << "Error " << error_code << " : " << msg << endl;
}

template < class Cont, typename value_type>
// Allocated on the stack; works with most STL containers
value_type*	convert_stl_to_array ( const Cont & cont )
{
	value_type array[cont.size()];
	try
	{
    	std::copy(cont.begin(), cont.end(), array);
	}
	catch (std::exception & e)
	{
		string msg = e.what();
		msg.append(" Returning NULL");
		error_log("convert_stl_to_array", msg , errno );
	}
	return array;
}