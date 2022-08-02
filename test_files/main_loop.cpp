/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 11:52:15 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 14:28:30 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "typedef.hpp"
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using irc::t_pollfd;
using irc::Server;

int	run_server( Server& server )
{
	t_pollfd*	pollfds;
	int			i;

	while (server.get_exit_status() == false)
	{
		pollfds = server.poll_sockets();
		if (pollfds == NULL)
			continue;
		server.process_connections(pollfds[0]);
		if (server.get_client_count() > 0)
		{
			server.process_clients(&pollfds[1]);
		}
	}	
}

int	main( int argc, char** argv )
{
	if (argc != 3)
		return 1;
	try 
	{
		Server& server = Server::get_server(static_cast<u_int>(std::stoi(string(argv[1]))), argv[2]);
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
