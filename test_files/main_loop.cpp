/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadia <jbadia@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:52:15 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/05 10:36:16 by jbadia           ###   ########.fr       */
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

int	run_server( Server& server )
{
	t_pollfd*	pollfds;
	size_t		client_count;
	int			i;

	while (server.get_exit_status() == false)
	{
		client_count = server.get_client_count();
		pollfds = server.poll_sockets();
		if (pollfds == NULL)
			continue;
		server.process_connections(pollfds[0]);
		if (client_count > 0)
		{
			server.process_clients(&pollfds[1], client_count);
		}
	}
	cout << GREEN << "Leaving with absolute grace" << RESET << endl;
	close(server.get_fd()); // Need a close function
}

int	main( int argc, char** argv )
{
	if (argc != 3)
		return 1;
	try 
	{
		Server& server = Server::get_server(static_cast<u_int>(std::stoi(string(argv[1]))), argv[2]);
		server.set_signal_ctrl_c();
		// check if server did get created
		run_server(server);
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
