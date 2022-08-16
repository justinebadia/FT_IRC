#include "Server.hpp" // includes: <string><list><map><vector><exception><iostream> "irc_define.hpp"
					  // "../Client/Client.hpp" "Message.hpp" "typedef.hpp"

// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <poll.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <signal.h>

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

	_init_operators();
	CommandManager::_init_command_map();
	CommandManager::_init_reply_map();
	CommandManager::set_server(this);
	CommandManager::set_database(&_database);
	fcntl(this->get_fd(), F_SETFL, O_NONBLOCK);
	this->get_pollfd().events = POLLIN;
}

void	Server::_init_operators( void )
{
	_operator_vect.push_back(Operator("sfournie", "irc"));
	_operator_vect.push_back(Operator("jbadia", "irc"));
	_operator_vect.push_back(Operator("tshimoda", "irc"));
}

void	Server::_kill_server( void )
{
	disconnect_all_clients();
	close(get_fd());
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
	int			client_fd;
	
	if (pollfd.revents | POLLIN)
	{
		while(true)
		{
			socklen_t	len;
			client_fd = accept(pollfd.fd, reinterpret_cast<struct sockaddr*>(&_server_socket.addr6), &len);
			if (client_fd == -1)
				break;
			int	opt = 1;
			setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
			_database.add_client_list(Client(client_fd, _server_name));
			Server::log(string(GREEN) + "Server::process_connections: Added client with fd " + std::to_string(client_fd) + RESET); // WARNING
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

void	Server::_process_client_pollerr( const t_pollfd& pollfd )
{
	disconnect_client(pollfd.fd);
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
		disconnect_client(pollfd.fd);
		return ;
	}
	buffer[bytes] = '\0';
	client->append_buff(BUFFIN, string(buffer));
	Server::log(string(GREEN) + "Server::_process_client_pollin: received client fd "
				+ std::to_string(pollfd.fd) + ": " + RESET + client->get_buff(BUFFIN)); // WARNING
	if (!client->is_registered())
	{
		CommandManager::execute_commands_registration(*client);
		std::cout << "ADRESSE IP = " << client->get_client_ip() << std::endl;
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

void	Server::_process_client_pollout( const t_pollfd& pollfd )
{
	ssize_t		bytes;
	Client*		client;
	
	client = _database.get_client(pollfd.fd);
	if (!client)
		return ;
	if (client->get_buff(1).size() <= 0)
		return;
	Server::log(string(GREEN) + "Buff content before sending: " + client->get_buff(1).c_str() + RESET);
	bytes = send( pollfd.fd, client->get_buff(1).c_str(), client->get_buff(1).length(), MSG_DONTWAIT);
	if (bytes > 0)
		client->clear_buff(BUFFOUT); // POUR TESTER - À CHANGER
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
	}
	_kill_server();
	return 0;
}

void	Server::disconnect_client( const int& fd )
{
	//_database.remove_client_list(fd);
	
	Client* client = _database.get_client(fd);

	_database.delete_client_from_all_lists(client);
	close(fd);
	Server::log("Disconnected client of fd " + std::to_string(fd));
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

bool	Server::attempt_client_as_operator( Client& client, const string& oper_name, const string& oper_pass )
{
	t_operator_vect::iterator	it;

	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		if ((*it).client_fd == client.get_fd())
		{
			
			Server::log("Client nicknamed " + client.get_nickname() + " is already an operator");
			return false;
		}
	}
	for (it = _operator_vect.begin(); it != _operator_vect.end(); it++)
	{
		Server::log("checking " + (*it).name + " " + oper_name + " " + (*it).password + " " + oper_pass);
		if ((*it).client_fd > 0 && !(*it).name.compare(oper_name) && !(*it).password.compare(oper_pass))
		{
			(*it).client_fd = client.get_fd();
			client._set_operator(true);
			Server::log("Client nicknamed " + client.get_nickname() + " is now an operator");
			return true;
		}
	}
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

