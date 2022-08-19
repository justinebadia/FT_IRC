#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int main( void ) {

	char hostname[128];
	char ip[16];
	struct hostent* host;
	struct sockaddr_in sock_addr;

	gethostname(hostname, sizeof(hostname));
	printf("hostname: %s\n", hostname);

	host = gethostbyname(hostname);

	for (int i = 0; host->h_addr_list[i]; i++) {
		sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);
		inet_ntop(AF_INET, &sock_addr.sin_addr, ip, sizeof(ip));
		printf("[int i = %d] [ip address = %s]\n", i, ip);
	}

	return 0;
}
