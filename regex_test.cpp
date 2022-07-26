/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 16:45:07 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/25 18:00:16 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <regex>
#include <string>
#include "include/color.hpp"

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::regex;

int main()
{
	string		input("");
	string		str_cmp = "^(?![&#:$])((?![.,*?!@]).)*$";
	regex		reg_cmp(str_cmp);

	while (input.compare("exit"))
	{

		cout << endl << CYAN 				\
		<< " -----------------" << endl		\
		<< "| Enter a command |" << endl	\
		<< " -----------------" << endl		\
		<< RESET;

		cout << YELLOW << "regex <expression> "<< RESET << "to change regex. " \
		<<  "Current : " << YELLOW << str_cmp << endl;
		cout << GREEN << "<string> " << RESET << "to compare " << GREEN \
		<< "<string>" << RESET << " to " << YELLOW << str_cmp << RESET << endl << endl;

		cout << CYAN "Your input : " << RESET;
		std::getline(std::cin, input);
		
		if (!input.compare(0, 6, "regex "))
		{
			reg_cmp = regex(&input[6]);
			str_cmp = &input[6];
			cout << "The new regex is " << &input[6];
		}
		else if (input.compare("exit"))
		{
			cout << endl << CYAN 				\
			<< " ---------" << endl		\
			<< "| Testing |" << endl	\
			<< " ---------" << endl		\
			<< RESET;
			cout << GREEN << "string : " << input << endl;
			cout << YELLOW << "regex  : " << str_cmp << endl;

			cout << endl;
			if (std::regex_match(input, reg_cmp))
				cout << GREEN << "Input matches!" << endl;
			else
				cout << RED << "Input does not match..." << endl;
			cout << RESET;
		}

		
	}
	
}