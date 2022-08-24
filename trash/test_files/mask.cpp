/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mask.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 16:45:07 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/14 22:43:03 by fousse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include "../includes/color.hpp"

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::list;

bool	compare_to_mask( string mask, string str )
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
		cout << mask[mask_i] << " " << str[str_i] << endl;
		while (mask[mask_i] == '*')
			mask_i++;
		cout << mask[mask_i] << " " << str[str_i] << endl;
		if (!mask[mask_i])
			return true;
		next_wild = mask.find("*", mask_i);
		if (next_wild == string::npos)
			next_wild = mask.length() - 1;
		sublen = next_wild - mask_i;
		find_mask = str.find(mask.substr(mask_i, sublen), str_i);
		if (find_mask == string::npos)
			return false;
		mask_i += sublen + 1;
		str_i = find_mask + sublen;
	}
	return true;
}
		

int main()
{
	string		input("");
	string		mask("");

	while (input.compare("exit"))
	{

		cout << endl << CYAN 				\
		<< " -----------------" << endl		\
		<< "| Enter a command |" << endl	\
		<< " -----------------" << endl		\
		<< RESET;

		cout << YELLOW << "mask <expression> "<< RESET << "to change mask. " \
		<<  "Current : " << YELLOW << mask << endl;
		cout << GREEN << "<string> " << RESET << "to compare " << GREEN \
		<< "<string>" << RESET << " to " << YELLOW << mask << RESET << endl << endl;

		cout << CYAN "Your input : " << RESET;
		std::getline(std::cin, input);
		
		if (!input.compare(0, 5, "mask "))
		{
			mask = &input[5];
			cout << "The new mask is " << &input[5];
		}
		else if (input.compare("exit"))
		{
			cout << endl << CYAN 				\
			<< " ---------" << endl		\
			<< "| Testing |" << endl	\
			<< " ---------" << endl		\
			<< RESET;
			cout << GREEN << "string : " << input << endl;
			cout << YELLOW << "mask  : " << mask << endl;

			cout << endl;
			if (compare_to_mask(mask, input))
				cout << GREEN << "Input matches!" << endl;
			else
				cout << RED << "Input does not match..." << endl;
			cout << RESET;
		}

		
	}
	
}