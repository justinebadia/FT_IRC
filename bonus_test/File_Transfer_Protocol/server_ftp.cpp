/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ftp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 09:21:12 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/07 10:46:51 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>	// inet_addr()
#include <netinet/in.h>	// struct sockaddr_in
#include <stdio.h>		// fgets() fopen() fprintf() perror() printf() struct FILE
#include <stdlib.h>
#include <string>		// bzero()
#include <sys/socket.h>	// accept() bind() listen() recv() send() socket()
#include <unistd.h>		// close()

#define SIZE 1080
#define SUCCESS 0
#define FAIL -1

void	write_file(int new_socket)
{
	FILE* fp;
	
	int n;
	std::string	filename = "newly_written_story.txt";
	char	buffer[SIZE];

	fp = fopen(filename.c_str(), "w");
	if (fp == NULL)
	{
		perror("Error: creating new file.");
		exit(FAIL);
	}

	while (true)
	{
		n = recv(new_socket, buffer, SIZE, 0);
		if (n <= 0)
		{
			break ;
			return;
		}
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
	}
}


int	main( void )
{
	std::string	ip = "127.0.0.1";
	int 		port = 6667;
	int			res;

	int					server_socket;
	struct sockaddr_in	server_addr;

	int					new_socket;
	struct sockaddr_in	new_addr;

	socklen_t			addr_size;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == FAIL)
	{
		perror("Error: socket()");
		exit(FAIL);
	}
	printf("server_socket created\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

	res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (res == FAIL)
	{
		perror("Error: bind()");
		exit(FAIL);
	}
	printf("server_socket bind()ing successful\n");

	res = listen(server_socket, 10);
	if (res == SUCCESS)
	{
		printf("listen()ing...\n");
	}
	else
	{
		perror("Error: listen()");
		exit(FAIL);
	}
	addr_size = sizeof(new_addr);
	new_socket = accept(server_socket, (struct sockaddr*)&new_socket, &addr_size);

	write_file(new_socket);

	printf("Writting data in new file successful!\n");
}
