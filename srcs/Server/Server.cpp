#include "Server.hpp" // includes: <string><list><map><vector><utility><signal.h><exception><iostream> "irc_define.hpp"
					  // <arpa/inet.h><netinet/in.h><sys/types.h><sys/socket.h>
					  // "../Client/Client.hpp" "Message.hpp" "typedef.hpp"

#include <poll.h>
#include <unistd.h>
#include <sys/fcntl.h>
// #include "commands.hpp"
#include "replies.hpp"
#include "numeric_replies.hpp"
#include "../includes/color.hpp"


using namespace irc;
using std::cout;
using std::cerr;
using std::endl;

int Server::log_level = 1;

/*WARNING: move set_exit_true() in a different file*/
void	set_exit_true( int signal ) 
{
	(void)signal;
	Server::get_server().set_exit(true);
} /*/


/*---------------PROHIBITED-CONSTRUCTORS--------------*/

Server::Server( void ) : _port(PORT), _password("") {}		// default constructor [PRIVATE]

Server::Server( const Server& other ) 						// copy constructor [PRIVATE]
	: _server_socket(other._server_socket)
	, _server_name(other._server_name)
	, _port(other._port)
	, _password(other._password)
	, _exit(other._exit)
	{ *this = other; }

Server&	Server::operator=( const Server& other ){}			// copy operator overload [PRIVATE]


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Server::Server( const unsigned int& port, const string password, bool exit ) // main server constructor
	: _server_name(HOSTNAME)	// 127.0.0.1 
	, _port(port)				// 6667
	, _password(password)
	, _exit(false)
{
	init_server();
}

Server::~Server( void )										// default destructor
{}


/*--------------------------PRIVATE-MEMBER-FUNCTIONS--------------------------*/

void	Server::_process_client_pollerr( const t_pollfd& pollfd )
{
	_disconnect_client(pollfd.fd);
	cout << GREEN << "Server::_process_client_pollerr: removed client fd " << RESET << endl; // WARNING
}

void	Server::_process_client_pollin( const t_pollfd& pollfd )
{
	char		buffer[MAX_IN + 1];
	ssize_t		bytes;
	Client		*client;
	
	client = _database.get_client(pollfd.fd);

	bytes = recv( pollfd.fd, buffer, MAX_IN, MSG_DONTWAIT );
	if (bytes <= 0)
	{
		_disconnect_client(pollfd.fd);
		return ;
	}
	buffer[bytes] = '\0';
	client->append_buff(BUFFIN, string(buffer));
	cout << endl << "Buffin string as ascii : "; // WARNING
	for (int i=0; i<bytes; i++)
    	cout << std::hex << (int)buffer[i];
	cout << endl;
	// if (!client->get_nickname().compare("operator"))
	// {
	// 	CommandManager::send_to_clients(_database.get_client_ptr_list(), client->get_buff(BUFFIN) + "\r\n");
	// 	client->clear_buff(BUFFIN);
	// 	return;
	// }
	cout << GREEN << "Server::_process_client_pollin: received and appended for client fd " << pollfd.fd << ": " << RESET << client->get_buff(BUFFIN)  << endl; // WARNING
	if (!client->is_registered())
	{
		CommandManager::execute_commands_registration(*client);
		_check_registration(client);
	}
	else
		CommandManager::execute_commands(*client);
	client->clear_buff(BUFFIN);
}

void	Server::_check_registration( Client* client )
{
	if (!client->is_registered() && client->is_nickname_set() && client->is_username_set()) // WARNING missing password check
	{
		Message	message(client);
		client->set_registration_flags(Client::COMPLETE); 
		CommandManager::get_reply_ptr(RPL_WELCOME)(message); //WARNING
		client->append_buff(BUFFOUT, message.get_message_out());
		client->append_buff(BUFFOUT, "\r\n");
	}
}

void	Server::_process_client_pollout( const t_pollfd& pollfd )
{
	ssize_t		bytes;
	Client*		client;
	
	client = _database.get_client(pollfd.fd);
	if (!client)
		return ;
	if (client->get_buff(1).size() <= 0)
		return;
	cout << GREEN <<"Buff content before sending: " << client->get_buff(1).c_str() << RESET <<endl;
	bytes = send( pollfd.fd, client->get_buff(1).c_str(), MAX_OUT, MSG_DONTWAIT);
	if (bytes > 0)
		client->clear_buff(BUFFOUT); // POUR TESTER - A SUPPRIMER
	// client->trim_buff(1, static_cast<size_t>(bytes));
}

void	Server::_disconnect_client( const int& fd )
{
	_database.remove_client_list(fd);
	close(fd);
}

/*---------------------------------GETTERS-----------------------------------*/

Server&				Server::get_server( const unsigned int& port, const string password, bool exit ) // singleton
{
	static Server singleton(port, password, exit); // static singleton declared on the stack, call the main constructor
	return singleton;
}

const t_socket&			Server::get_socket( void ) const { return _server_socket; }
const string&			Server::get_name( void ) const { return _server_name; }
const unsigned int		Server::get_port( void ) const { return _port; }
Database*		 		Server::get_database( void ) { return &_database; }
const string&			Server::get_password( void ) const { return _password; }
bool					Server::get_exit_status( void ) const { return _exit; }

t_pollfd&				Server::get_pollfd( void ){ return _server_socket.pollfd; }
t_addr6&				Server::get_addr6( void ){ return _server_socket.addr6; }
const int&				Server::get_fd( void ) const { return _server_socket.pollfd.fd; }

t_pollfd*	Server::get_pollfd_array( void ) // Needs to be freed
{
	t_client_list			client_list;
	t_client_list::iterator it;
	t_pollfd*				pollfd_array;
	size_t					size = _database.get_client_count() + 1;
	size_t					i = 1;

	if (size == 0)
		return NULL;
	else
	{
		client_list = _database.get_client_list();
		pollfd_array = new t_pollfd[size];
		if (!pollfd_array)
			return NULL;
		pollfd_array[0] = get_socket().pollfd;
		for (it = client_list.begin(); it != client_list.end() && i < size; it++)
		{
			pollfd_array[i++] = (*it).get_pollfd();
		}
	}
	return pollfd_array;
}


/*---------------------------------SETTERS-----------------------------------*/

void	Server::set_fd( int fd )
{
	_server_socket.pollfd.fd = fd;
}

void	Server::set_exit( bool status )
{
	_exit = status;
}

void	Server::set_signal_ctrl_c( void )
{
	signal(SIGINT, &set_exit_true);
}

/*--------------------------OTHER-MEMBER-FUNCTIONS---------------------------*/

int	Server::run_server( void )
{
	t_pollfd*	pollfds;
	size_t		client_count;
	int			i;

	while (get_exit_status() == false)
	{
		client_count = _database.get_client_count();
		pollfds = poll_sockets();
		if (pollfds == NULL)
			continue;
		process_connections(pollfds[0]);
		if (client_count > 0)
		{
			process_clients(&pollfds[1], client_count);
		}
	}
	cout << GREEN << "Leaving with absolute grace" << RESET << endl;
	close(get_fd()); // Need a close function
}

void	Server::init_server( void )
 {
	this->set_fd(socket(AF_INET6, SOCK_STREAM, 0));
	if (this->get_fd() == FAIL)	// Step 1:  socket()
	{
		throw Server::SocketErrorException();
	}

	this->get_pollfd().events = 0;
	this->get_pollfd().revents = 0;

	int opt; // to store the setsockopt options
	if (setsockopt(this->get_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == FAIL) // Step 2: setsockopt()
	{
		throw Server::SetsockoptErrorException();
	}
	
	this->get_addr6().sin6_family = AF_INET6; //change to 6 eventually
	this->get_addr6().sin6_addr = in6addr_any;
	this->get_addr6().sin6_port = htons(get_port());
	
	if ((bind(this->get_fd(), (struct sockaddr*)&this->get_addr6(), sizeof(this->get_addr6())) == FAIL)) // Step 3: bind()
	{
		throw Server::BindErrorException();
	}

	if (listen(this->get_fd(), MAX_PENDING) == FAIL) // Step 4: listen()
	{
		throw Server::ListenErrorException();
	}

	CommandManager::_init_command_map();
	CommandManager::_init_reply_map();
	CommandManager::set_server(this);
	CommandManager::set_database(&_database);
	fcntl(this->get_fd(), F_SETFL, O_NONBLOCK);
	this->get_pollfd().events = POLLIN;
}

t_pollfd*	Server::poll_sockets( void ) //needs to be deleted
{
	t_pollfd*	pollfd_array;

	pollfd_array = get_pollfd_array();
	if (!pollfd_array)
		return NULL;
	poll(pollfd_array, static_cast<nfds_t>(_database.get_client_count() + 1), 0);
	return pollfd_array;
}

void	Server::process_connections( const t_pollfd& pollfd )
{
	int			client_fd;
	
	if (pollfd.revents | POLLIN)
	{
		while(true)
		{
			socklen_t	len;
			client_fd = accept(pollfd.fd, reinterpret_cast<struct sockaddr*>(&_server_socket.addr6), &len);
			if (client_fd == -1)
				break;
			_database.add_client_list(Client(client_fd));
			cout << GREEN << "Server::process_connections: Added client with fd " << client_fd << RESET <<  endl; // WARNING
		}
	}
}

void	Server::process_clients( const t_pollfd* pollfd_array, size_t size )
{
	int			i;
	for (i = 0; i < size; i++)
	{
		if (pollfd_array[i].revents & POLLERR || pollfd_array[i].revents & POLLHUP)
		{
			_process_client_pollerr( pollfd_array[i] );
			continue;
		}
		if (pollfd_array[i].revents & POLLIN)
		{
			_process_client_pollin( pollfd_array[i]);
		}
		//_process_client_messages
		if (pollfd_array[i].revents & POLLOUT)
		{
			_process_client_pollout(pollfd_array[i]);
		}
		_database.clean_database();
	}
}

void	Server::log( const string& msg )
{
	if (log_level == 1)
		cout << msg << endl;
}

/*-------------------------NESTED-CLASS-EXCEPTIONS--------------------------*/

const char*	Server::SocketErrorException::what() const throw()
{
	return "Error: socket()";
}

const char*	Server::SetsockoptErrorException::what() const throw()
{
	return "Error: setsockopt()";
}

const char*	Server::BindErrorException::what() const throw()
{
	return "Error: bind()";
}

const char*	Server::ListenErrorException::what() const throw()
{
	return "Error: listen()";
}

