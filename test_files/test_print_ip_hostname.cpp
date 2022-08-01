#include <stdio.h>
#include <iostream>


#include "../srcs/Server/Server.hpp"

int	main( int argc, char **argv )
{
	if (argc != 3)
	{
		std::cout << "Please enter: ./ircserver <PORT> <PASSWORD>" << std::endl;
		return FAIL;
	}
	if (PORT != argv[1])
	{
		std::cout << "Please use port: 6667" << std::endl;
		return FAIL;
	}
	if (strlen(argv[2] <=1)
	{
		std::cout << "Invalid password" << std::endl;
		return FAIL;
	}
	
//STEP 1 GETADDRINFO()

	memset(&irc._hints, 0, sizeof(irc._hints));
 	irc._hints.ai_family  = AF_UNSPEC; // either IPv4 or IPv6
 	irc._hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
 	irc._hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	int err;
 	if ((err = getaddrinfo(127.0.0.1, PORT, &irc._hints, &irc._res)) != 0) // getaddrinfo()
 	{
   		printf("error %d\n", err);
   		return FAIL;
 	}

	std::string password(argv[2]);
	
	Server& irc(PORT, password); // instanciating a Server, calling its main constructor

	irc.set_signal_ctrl_c(server);

//STEP 2 SOCKET()

	irc._server_socket.pollfd.fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // socket()
 	
	if (irc._server_socket.pollfd.fd == FAIL)
	{
		std::cout << "Error: creating socket()" << std::endl;
		return FAIL;
	}

//STEP 3 SETSOCKOPT()

//	if (setsockopt(irc._server_socket.pollfd.fd, (struct sockaddr*)&address, sizeof(add
	



	irc._address.sin_family = AF_INET;
	irc._address.sin_addr.s_addr = INADDR_ANY;
	irc._address.sin_port = htons(PORT);



	irc._addr.s_addr = ((struct sockaddr_in *)(irc._res->ai_addr))->sin_addr.s_addr;

 	printf("ip address : %s\n", inet_ntoa(irc._addr));

 	freeaddrinfo(irc._res);

 	return SUCCESS;
}
