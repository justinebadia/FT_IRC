/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_ftp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tshimoda <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 09:55:24 by tshimoda          #+#    #+#             */
/*   Updated: 2022/08/07 10:42:04 by tshimoda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>	// inet_addr()
#include <netinet/in.h>	// struct sockaddr_in
#include <stdio.h>		// fgets() fopen() fprintf() perror() printf() struct FILE
#include <string>		// bzero()
#include <sys/socket.h>	// accept() bind() listen() recv() send() socket()
#include <unistd.h>		// close()

#define SIZE 1080
#define SUCCESS 0
#define FAIL -1

void	send_file( FILE* fp, int server_socket )
{
	char	data[SIZE] = {0};
	
	while (fgets(data, SIZE, fp) != NULL)
	{
		if (send(server_socket, data, sizeof(data), 0) == FAIL)
		{
			perror("Error: send()ing data");
			exit(FAIL);
		}
		bzero(data, SIZE);
	}
	printf("send()ing file data successful!\n");
}

int	main( void )
{
	std::string			ip = "127.0.0.1";
	int					port = 6667;
	int					res;

	int					server_socket;
	struct sockaddr_in	server_addr;

	FILE*				fp;
	std::string			filename = "story.txt";

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

	res = connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (res == FAIL)
	{
		perror("Error: connect()");
		exit(FAIL);
	}
	printf("server_socket connect()ing successful\n");

	fp = fopen(filename.c_str(), "r");
	if (fp == NULL)
	{
		perror("Error: fopen() and read file");
		exit(FAIL);
	}
	printf("fopen()ing file successful!\n");
	
	send_file(fp, server_socket);

	close(server_socket);
	printf("Client disconnected from the server_socket\n");

	return SUCCESS;
}

