// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6667
  
int main(int argc, char const* argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
	struct pollfd pollfds[1];
	std::string	input;		

    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        printf("\n Socket creation error \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

	pollfds[0].fd = sock;
	pollfds[0].events = POLLIN | POLLOUT;
	int res;
	while(1)
	{
		if (!input.size())
        {
			std::getline(std::cin, input);
            input.append("\r\n");
        }
        
		res = poll(pollfds, 1, 0);
		if (res != 0)
		{
			if ((pollfds[0].revents & POLLIN) == POLLIN)
			{
				valread = read(pollfds[0].fd, buffer, 1024);
				if (valread > 0)
					printf("\nRead : %s\n", buffer);
			}
			if (pollfds[0].revents & POLLOUT && input.size())
			{
				if (send(pollfds[0].fd, input.c_str(), input.size(), MSG_DONTWAIT))
					input.clear();
			}
		}
	}
    // send(sock, hello, strlen(hello), 0);
//    printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
  
    // closing the connected socket
    close(client_fd);
	return (0);
}
