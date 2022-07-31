/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 13:46:08 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 13:56:31 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <map>
#include <list>
#include <vector>

#include "Socket.hpp"

#define PORT 6666

using std::string;
// using irc::Socket;
// using irc::t_sockaddr6;
// using irc::t_pollfd;

typedef struct pollfd t_pollfd;

void	error_exit(const char * msg_error, int error = 1)
{
	perror(msg_error);
    exit(error);
}

// void	close_sockets(std::vector<t_pollfd>	& socket_fds)
// {
// 	for (std::vector<t_pollfd>::iterator i; i != socket_fds.end(); i++)
// 	{
// 		close((*i).fd);
// 	}
// }

int main(int argc, char **argv)
{
	int		server_fd;
	int		port;
	int		bytes_read;
	bool	_exit = false;
	char	buffer[1025];
	string	password;

	int						new_sock;
	t_pollfd				socket_fds[1000];
	struct	sockaddr_in		address;
	int						connections = 0;
	int						poll_n;



	port = PORT;
	password = argv[2];
	server_fd = socket(AF_INET, SOCK_STREAM, 0); // attempt to open a socket for the server
	if (server_fd <= 2)
		error_exit("socket failed", EXIT_FAILURE);

	int opt; // to store the setsockopt options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) // attempt to set options on the socket
		error_exit("setsockopt", EXIT_FAILURE);
		
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
	// Forcefully attaching socket to the PORT
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		error_exit("bind failed", EXIT_FAILURE);
	
	if (listen(server_fd, 3) < 0) 
		error_exit("listen", EXIT_FAILURE);

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	int	broadcast_timer = 10000;
	while (!_exit)
	{
		// check for incoming connections
		new_sock = accept(server_fd, NULL, NULL);
		while (new_sock > 0)
		{
			printf("\n new connection with fd=%d\n", new_sock);
			
			socket_fds[connections] = t_pollfd();
			socket_fds[connections++].fd = new_sock;
			socket_fds[connections++].events = POLLIN | POLLOUT;

			new_sock.fd = accept(server_fd, NULL, NULL);
		}

		// poll if there are clients
		if (connections > 0)
		{
			poll_n = poll(socket_fds, connections, 0);
			
		}
		if (poll_n > 0)
		{
			for (int i = 0; i < connections; i++)
			{
				if (socket_fds[i].revents & POLLIN)
				{
					bytes_read = recv(socket_fds[i].fd, buffer, 1025, MSG_DONTWAIT);
					if (bytes_read > 0)
					{
						buffer[bytes_read] = '\0';
						printf("\nClient message : %s\n", buffer);
						if (std::string(buffer).compare("exit"))
						{
							break;
						}

					}
				}
				if (broadcast_timer-- == 0)
				{
					if (socket_fds[i].revents & POLLOUT)
					{
						send(socket_fds[i].fd, "serverdata", 10, MSG_DONTWAIT);
					}
					broadcast_timer = 10000;
				}
			}
		}
	}
	// close_sockets(socket_fds);



	
	
}
