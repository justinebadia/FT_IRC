#include "Server.hpp" // includes: <string><list><map><vector><exception><iostream> "irc_define.hpp"
					  // "../Client/Client.hpp" "Message.hpp" "typedef.hpp"

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

#include "CommandManager.hpp"
#include "Message.hpp"
#include "irc_define.hpp"
#include "color.hpp"
#include "typedef.hpp"
#include "numeric_replies.hpp"
#include "utils.hpp"

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
}

/*---------------PROHIBITED-CONSTRUCTORS--------------*/

Server::Server( void ) // default constructor [PRIVATE]
	: _server_ip(grab_ip_address())
	, _port(PORT)
	, _password("") 
	{}		

Server::Server( const Server& other ) 						// copy constructor [PRIVATE]
	: _server_socket(other._server_socket)
	, _server_ip(other._server_ip)
	, _server_name(other._server_name)
	, _port(other._port)
	, _password(other._password)
	, _exit(other._exit)
	{ *this = other; }

Server&	Server::operator=( const Server& ){ return *this; }			// copy operator overload [PRIVATE]


/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Server::Server( const unsigned int& port, const string password, bool exit ) // main server constructor
	: _server_ip(grab_ip_address())
	, _server_name(_server_ip)
	, _port(port)				// 6667
	, _password(password)
	, _exit(exit)
{
	_init_server();
}

Server::~Server( void )										// default destructor
{}


/*--------------------------PRIVATE-MEMBER-FUNCTIONS--------------------------*/

void	Server::_init_server( void )
 {
	this->set_fd(socket(AF_INET6, SOCK_STREAM, 0));
	if (this->get_fd() == FAIL)	// Step 1:  socket()
	{
		throw Server::SocketErrorException();
	}

	this->get_pollfd().events = 0;
	this->get_pollfd().revents = 0;

	int opt = 1; // to fix the binding problem, opt has to NOT be 0
	if (setsockopt(this->get_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == FAIL) // Step 2: setsockopt()
	{
		throw Server::SetsockoptErrorException();
	}
	
	this->get_addr6().sin6_family = AF_INET6;
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

	_init_operators();
	CommandManager::_init_command_map();
	CommandManager::_init_reply_map();
	CommandManager::set_server(this);
	CommandManager::set_database(&_database);
	fcntl(this->get_fd(), F_SETFL, O_NONBLOCK);
	this->get_pollfd().events = POLLIN;
	std::time(&this->_last_ping);
}

void	Server::_kill_server( void )
{
	disconnect_all_clients();
	close(get_fd());
}

void	Server::_check_for_kills( void )
{
	t_client_ptr_list			clients;
	t_client_ptr_list::iterator	it;

	clients = _database.get_client_ptr_list();
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->get_to_be_killed())
		{
			if ((*it)->get_buff(BUFFOUT).empty())
				disconnect_client((*it)->get_fd());
		}
	}
}

void	Server::_check_for_timeouts( void )
{
	t_client_ptr_list			clients;
	t_client_ptr_list::iterator	it;
	time_t						current_time;

	clients = _database.get_client_ptr_list();
	std::time(&current_time);
	// Server::log("TIME - LAST_PING: " + std::to_string(current_time - _last_ping));
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (current_time - (*it)->get_last_read() >= CLIENT_TIMEOUT)
		{
			if ((*it)->get_buff(BUFFOUT).empty())
				disconnect_client((*it)->get_fd());
		}
	}
	if ((current_time - _last_ping) >= PING_INTERVAL)
	{
		CommandManager::send_to_clients(clients, ": PING" + CRLF);
		_last_ping = current_time;
	}
}

void	Server::_init_operators( void )
{
	_operator_vect.push_back(Operator("sfournie", "irc"));
	_operator_vect.push_back(Operator("jbadia", "irc"));
	_operator_vect.push_back(Operator("tshimoda", "irc"));
}

void	Server::_clean_operators( void )
{
	t_operator_vect::iterator	it;

	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		if ((*it).client_fd != -1 && _database.get_client((*it).client_fd) == NULL)
		{
			Server::log("Operator " + (*it).name + " has been cleaned");
			(*it).client_fd = -1;
		}
	}
}



t_pollfd*	Server::_poll_sockets( void ) //needs to be deleted
{
	t_pollfd*	pollfd_array;

	pollfd_array = get_pollfd_array();
	if (!pollfd_array)
		return NULL;
	poll(pollfd_array, static_cast<nfds_t>(_database.get_client_count() + 1), 0);
	return pollfd_array;
}

void	Server::_process_connections( const t_pollfd& pollfd )
{
	Client		client;
	t_addr6*	cli_addr6;
	int			client_fd;
	
	if (pollfd.revents | POLLIN)
	{
		while(true)
		{
			cli_addr6 = &client.get_addr6_ref();
			socklen_t	len = sizeof(*cli_addr6);
			char ip[INET6_ADDRSTRLEN];

			client_fd = accept(pollfd.fd, reinterpret_cast<struct sockaddr*>(cli_addr6), &len);
			if (client_fd == -1)
				break;
			int	opt = 1;
			setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
			client.set_fd(client_fd);
			if (cli_addr6->sin6_family == AF_INET6)
				inet_ntop(AF_INET6, &cli_addr6->sin6_addr, ip, INET6_ADDRSTRLEN);
			else
				inet_ntop(AF_INET, &cli_addr6->sin6_addr, ip, INET_ADDRSTRLEN);
			client.set_ip(convert_ip_address(ip));
			_database.add_client_list(client);

			// socklen_t	len = 0;
			// t_socket	client_sock = t_socket();
			
			// client_fd = accept(pollfd.fd, reinterpret_cast<struct sockaddr*>(&_server_socket.addr6), &len);
			// if (client_fd == -1)
			// 	break;
			// int	opt = 1;
			// setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
			// _database.add_client_list(Client(client_fd, _server_name));

			
			Server::log("Newly accepted client ip = " + client.get_client_ip());
		}
	}
}

void	Server::_process_clients( const t_pollfd* pollfd_array, size_t size )
{
	size_t	i;

	for (i = 0; i < size; i++)
	{
		if (pollfd_array[i].revents & POLLERR || pollfd_array[i].revents & POLLHUP)
		{
			_process_client_pollerr( pollfd_array[i] );
			continue;
		}
		_process_client_pollin( pollfd_array[i]);
		//_process_client_messages
		if (pollfd_array[i].revents & POLLOUT)
		{
			_process_client_pollout(pollfd_array[i]);
		}
		_database.clean_database();
		_check_for_timeouts();
		_check_for_kills();
		_clean_operators();
	}
}

void	Server::_process_client_pollerr( const t_pollfd& pollfd )
{
	disconnect_client(pollfd.fd);
	Server::log("Server::_process_client_pollerr: removed client fd " + std::to_string(pollfd.fd)); // WARNING
}

void	Server::_process_client_pollin( const t_pollfd& pollfd )
{
	char		buffer[MAX_IN + 1];
	ssize_t		bytes;
	Client		*client;
	
	client = _database.get_client(pollfd.fd);
	
	if (!client || client->get_to_be_killed())
		return ;
	if (pollfd.revents & POLLIN)
	{
		bytes = recv( pollfd.fd, buffer, MAX_IN, MSG_DONTWAIT );
		if (bytes <= 0)
			return disconnect_client(pollfd.fd);
		client->set_last_read_to_now();
		buffer[bytes] = '\0';
		client->append_buff(BUFFIN, string(buffer));
		Server::log(string(YELLOW) + "_process_client_pollin: content received from client fd "
				+ std::to_string(pollfd.fd) + ":\n" + RESET + client->get_buff(BUFFIN)); // WARNING
	}
	if (!client->get_buff(BUFFIN).empty())
	{
		CommandManager::execute_commands(client);
		// Server::log(string(YELLOW) + "_process_client_pollin: buffin processed successfully");
	}
}

void	Server::_process_client_pollout( const t_pollfd& pollfd )
{
	ssize_t		bytes;
	Client*		client;
	
	client = _database.get_client(pollfd.fd);
	if (!client || client->get_buff(1).size() <= 0)
		return;
	Server::log(string(YELLOW) + "Buff content before sending to client fd " 
				+ std::to_string(pollfd.fd) + ":\n" + RESET + client->get_buff(BUFFOUT));
	bytes = send( pollfd.fd, client->get_buff(1).c_str(), client->get_buff(1).length(), MSG_DONTWAIT);
	if (bytes > 0)
		client->get_buff(BUFFOUT).erase(0, static_cast<unsigned long>(bytes));
	Server::log(string(YELLOW) + "Buff content sent!");
	// client->trim_buff(1, static_cast<size_t>(bytes));
}

/*---------------------------------GETTERS-----------------------------------*/

Server&				Server::get_server( const unsigned int& port, const string password, bool exit ) // singleton
{
	static Server singleton(port, password, exit); // static singleton declared on the stack, call the main constructor
	return singleton;
}

const t_socket&		Server::get_socket( void ) const { return _server_socket; }
const string&		Server::get_name( void ) const { return _server_name; }
string				Server::get_prefix( void ) const { return (":" + _server_name + " "); }
const unsigned int&	Server::get_port( void ) const { return _port; }
Database*		 	Server::get_database( void ) { return &_database; }
const string&		Server::get_password( void ) const { return _password; }
bool				Server::get_exit_status( void ) const { return _exit; }
const string&		Server::get_server_ip( void ) const { return _server_ip; }

t_pollfd&			Server::get_pollfd( void ){ return _server_socket.pollfd; }
t_addr6&			Server::get_addr6( void ){ return _server_socket.addr6; }
const int&			Server::get_fd( void ) const { return _server_socket.pollfd.fd; }

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

	while (get_exit_status() == false)
	{
		client_count = _database.get_client_count();
		pollfds = _poll_sockets();
		if (pollfds == NULL)
			continue;
		_process_connections(pollfds[0]);
		if (client_count > 0)
		{
			_process_clients(&pollfds[1], client_count);
		}
		if (pollfds)
			delete [] pollfds;
	}
	_kill_server();
	return 0;
}

void	Server::check_registration( Client* client )
{
	if (!client->is_registered() && client->is_nickname_set() && client->is_username_set()) // WARNING missing password check
	{
		Message					message(client);
		t_reply_function_ptr	reply;

		client->set_registration_flags(Client::COMPLETE); 
		reply = CommandManager::get_reply_ptr(RPL_WELCOME); //WARNING
		if (reply)
			reply(message);
		client->append_buff(BUFFOUT, message.get_message_out());
		client->append_buff(BUFFOUT, "\r\n");
	}
}

void	Server::disconnect_client( const int& fd )
{
	//_database.remove_client_list(fd);
	
	Client* client = _database.get_client(fd);

	Server::log("Disconnected client of fd " + std::to_string(fd));
	if (fd > 0)
		close(client->get_fd());
	if (client)
		_database.delete_client_from_all_lists(client);
}

void	Server::disconnect_all_clients( void )
{
	t_client_ptr_list			client_list;
	t_client_ptr_list::iterator	it;
	int							fd;

	client_list = _database.get_client_ptr_list();
	for (it = client_list.begin(); it != client_list.end(); it++)
	{
		fd = (*it)->get_fd();
		_database.delete_client_from_all_lists(*it);
		close(fd);
	}
}

int		Server::attempt_client_as_operator( Client& client, const string& oper_name, const string& oper_pass )
{
	t_operator_vect::iterator	it;

	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		if ((*it).client_fd == client.get_fd())
		{
			Server::log("Client nicknamed " + client.get_nickname() + " is already an operator");
			return ERR_NOOPERHOST;;
		}
	}
	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		Server::log("checking " + (*it).name + " " + oper_name + " " + (*it).password + " " + oper_pass);
		if ((*it).client_fd != -1 && !(*it).name.compare(oper_name) && !(*it).password.compare(oper_pass))
			return ERR_NOOPERHOST;
		if ((*it).client_fd == -1)
		{
			if (!(*it).name.compare(oper_name))
			{
				if ((*it).password.compare(oper_pass))
					return ERR_PASSWDMISMATCH;
				(*it).client_fd = client.get_fd();
				client._set_operator(true);
				Server::log("Client nicknamed " + client.get_nickname() + " is now an operator");
				return RPL_YOUREOPER;
			}
		}
	}
	return ERR_NOOPERHOST;
}

bool	Server::is_client_operator( Client* client )
{
	if (client)
		return is_client_operator(client->get_fd());
	return false;
}

bool	Server::is_client_operator( const string& nickname )
{
	Client* client = _database.get_client(nickname);

	if (client)
		return is_client_operator(client->get_fd());
	return false;
}

bool	Server::is_client_operator( const int& fd )
{
	t_operator_vect::iterator	it;

	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		if ((*it).client_fd == fd)
			return true;
	}
	return false;
}

void	Server::log( const string& msg )
{
	if (log_level == 1)
		cout << GREEN << "Server log: " << RESET << msg << endl;
}

void	Server::log_error( const string& msg )
{
	if (log_level == 1)
		cout << RED << "Server log: " << RESET << msg << endl;
}

const string	 Server::grab_ip_address( void )
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
	return (string(ip));
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

