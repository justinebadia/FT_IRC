#include <climits>
#include <cstdint>
#include <sys/_types/_in_addr_t.h>
#include <sys/_types/_off_t.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int			socket( int domain, int type, int protocol );
int			setsockopt( int socket, int level, int option_name, const void* option_value, socklen_t option_len );
int			getsockname( int socket, struct sockaddr* restrict address, socklen_t* restrict address_len );
struct		protoent* getprotobyname( const char* name );
struct		gethostbyname( const char* name );
int			getaddrinfo( const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res );
void		freeaddrinfo( struct addrinfo* ai );
int			bind( int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int			connect( int socket, const struct sockaddr* address, socklen_t address_len );
int			listen( int socket, int backlog );
int			accept( int socket, struct sockaddr* restrict address, socklen_t* restrict address_len );
ssize_t		send( int socket, const void* buffer, size_t length, int flags ); //flag MSG_DONTWAIT enables nonblocking operation
ssize_t		recv( int socket, void* buffer, size_t length, int flags );

#include <arpa/inet.h>
uint16_t	htons( uint16_t hostshort );
uint32_t	htonl( uint32_t hostlong );
uint16_t	ntohs( uint16_t netshort );
uint32_t	ntohl( uint32_t netlong );
in_addr_t	inet_addr( const char* p );
char*		inet_ntoa( struct in_addr in );


#include <signal.h>
void		(*signal( int sig, void ( *func )( int )))( int );

#include <unistd.h>
off_t		lseek( int fildes, off_t offset, int whence );

#include <sys/stat.h>
int			fstat( int fildes, struct stat* buf );

#include <fcntl.h>
int			fcntl( int fildes, int cmd, ... );

#include <poll.h>
int			poll( struct pollfd fds[], nfds_t nfds, int timeout );