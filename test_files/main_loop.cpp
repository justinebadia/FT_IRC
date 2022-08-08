/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fousse <fousse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:52:15 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/07 21:44:45 by fousse           ###   ########.fr       */
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

int	main( int argc, char** argv )
{
	if (argc != 3)
		return 1;
	try 
	{
		Server& server = Server::get_server(static_cast<u_int>(std::stoi(string(argv[1]))), argv[2]);
		server.set_signal_ctrl_c();
		// check if server did get created
		server.run_server();
	}
	catch (std::exception& e)
	{
		cerr << "Error running the server: " << e.what() << endl;
		return 1;
	}
	
	// set_signal_ctrl_c();
	// signal(SIGINT, &
	return 0;
}
