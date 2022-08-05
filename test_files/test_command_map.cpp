/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_map.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:42:14 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 10:36:10 by jbadia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <map>

using std::string;

void	cmd_test( void * ptr)
{
	ptr = NULL;
	std::cout << "Successfully found NICK in the command map";
}

int main(int argc, char** argv)
{
	typedef			void(*f)(void*);
	std::map<string, f> cmd_map;
	f					fct;
	
	if (argc < 2)
		return 1;
	cmd_map.insert(std::make_pair(string("NICK"), &cmd_test));
	
	if (cmd_map.find(argv[1]) != cmd_map.end())
		cmd_map.find(argv[1])->second(NULL);
	else
		std::cout << argv[1] << " not found" << std::endl;
	return 0;
}