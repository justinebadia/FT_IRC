

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <time.h>

#include <string>
#include <exception>
#include <iostream>

using std::string;

int main( void )
{
	char hostname[128];
	char ip[16];
	struct hostent* host;
	struct sockaddr_in sock_addr;

	gethostname(hostname, sizeof(hostname));
	host = gethostbyname(hostname);

	for (int i = 0; host && host->h_addr_list[i]; i++) 
	{
		sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);
		inet_ntop(AF_INET, &sock_addr.sin_addr, ip, sizeof(ip));
	}
	// free(host);
	return 0;

	// char 	*hostname;
	// char 	*ip;
	// string	ip_str;
	// addrinfo		addr;
	// addrinfo		*res;
	// struct in_addr	addr6;
	// hostname = (char *)malloc(100);
	// ip = (char *)malloc(16);
	// memset(&addr, 0, sizeof(addr));
	// memset(&addr6, 0, sizeof(addr6));
	// // memset(hostname, 0, 100);
	// // memset(ip, 0, 16);
	// res = NULL;
	// gethostname(hostname, 100);
	// if (getaddrinfo(hostname, "6667", &addr, &res) != 0)
	// {
	// 	freeaddrinfo(res);
	// 	return 1;
	// }
	// addr6.s_addr = ((struct sockaddr_in *)(res[0].ai_addr))->sin_addr.s_addr;
	// inet_ntop(AF_INET, &addr6.s_addr, ip, INET_ADDRSTRLEN);
	// // freeaddrinfo(&addr);
	// freeaddrinfo(res);
	// free(hostname);
	// std::cout << ip << std::endl;
	// free(ip);
	// return 0;
}

