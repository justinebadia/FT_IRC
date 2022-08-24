/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:52:15 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/24 16:46:09 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "typedef.hpp"
#include <string>
#include <unistd.h>
#include "../includes/color.hpp"


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using irc::t_pollfd;
using irc::Server;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();

    while (it != s.end() && std::isdigit(*it))
		++it;
    return !s.empty() && it == s.end();
}

int	main( int argc, char** argv )
{
	int	port;

	if (argc != 3)
	{
		cerr << RED << "\v [Error: Usage:] " << RESET << "./ircserv <port> <pass>" << endl << "Valid port range: [6665-6669]\v" << endl;
		return FAIL;
	}
	try 
	{
		if (is_number(string(argv[1])) == false)
		{
			cerr << "[Error: port:] not a number\v" << endl;
			return FAIL;
		}
		port = std::stoi(string(argv[1]));
		if (port < 6665 || port > 6669)
		{
			cerr << "[Error: port:] Valid port range: [6665-6669]\v" << endl;
			return FAIL;
		}

		Server& server = Server::get_server(static_cast<u_int>(std::stoi(string(argv[1]))), argv[2]);
		server.set_signal_ctrl_c();
		// check if server did get created
		server.run_server();
	}
	catch (std::exception& e)
	{
		cerr << "Error running the server: " << e.what() << endl;
		return FAIL;
	}
	return SUCCESS;
}
