
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_index_overload copy.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42quebec.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:42:14 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 16:30:01 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

using std::string;

string		overload( int i , string _message_in)
{
	size_t start	= 0;
	size_t last		= 0;
	size_t next		= 0;
	size_t len		= _message_in.length();

	while (i-- >= 0)
	{   
		next = _message_in.find(" ", last);
		if (next == string::npos)
		{
			if (i > 0)
				return "";
			next = _message_in.length();
		} 
		start = last;
		len = next - last;
		last = next + 1;
	}
	return _message_in.substr(start, len);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		return 1;
	std::cout << overload(std::stoi(argv[1]), argv[2]);
	return 0;
}