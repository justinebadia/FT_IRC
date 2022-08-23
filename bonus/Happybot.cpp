
#include "Happybot.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <math.h>

using std::string;

/*============================================================================*/
namespace irc {

/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/

Happybot::Happybot( void )
{
	_init_Happybot();
}

Happybot::~Happybot( void )
{
	_happy_thoughts.clear();
	_grumpy_thoughts.clear();
}

Happybot&	Happybot::operator=( const Happybot& rhs )
{  
	_happy_thoughts = rhs._happy_thoughts;
	_grumpy_thoughts = rhs._grumpy_thoughts;
	_name = rhs._name;
	_mood = rhs._mood;
	_pollfd[0] = rhs._pollfd[0];
	return *this;

}

/*--------------------BOT-FUNCTIONS--------------------*/

void	Happybot::_init_Happybot( void )
{
	_mood = HAPPY;
	_name = "Happybot";
	srand(static_cast<unsigned int>(time(NULL)));
	_init_grumpy_thoughts();
	_init_happy_thoughts();
}

void	Happybot::_init_happy_thoughts( void )
{
	_happy_thoughts.push_back("Be healthy and take care of yourself, but be happy with the beautiful things that make you, you.");
	_happy_thoughts.push_back("The biggest adventure you can ever take is to live the life of your dreams.");
	_happy_thoughts.push_back("Beauty is everywhere. You only have to look to see it.");
	_happy_thoughts.push_back("You can't stop the waves, but you can learn to swim.");
	_happy_thoughts.push_back("Life is the dancer and you are the dance.");
	_happy_thoughts.push_back("The point is not to pay back kindness but to pass it on.");
	_happy_thoughts.push_back("Embrace the glorious mess that you are.");
	_happy_thoughts.push_back("Being happy never goes out of style.");
	_happy_thoughts.push_back("Happiness comes in waves. It'll find you again.");
	_happy_thoughts.push_back("The art of being happy lies in the power of extracting happiness from common things.");
}

void	Happybot::_init_grumpy_thoughts( void )
{
	_grumpy_thoughts.push_back("All alone! Whether you like it or not, alone is something you'll be quite a lot!");
	_grumpy_thoughts.push_back("But I didn't understand then. That I could hurt somebody so badly she would never recover. That a person can, just by living, damage another human being beyond repair.");
	_grumpy_thoughts.push_back("Y'all smoke to enjoy it. I smoke to die.");
	_grumpy_thoughts.push_back("Most people go through their whole lives, without ever really feeling that close with anyone");
	_grumpy_thoughts.push_back("From the moment we are born, we begin to die.");
	_grumpy_thoughts.push_back("Time doesn't heal all wounds. We both know that's bullshit; it comes from people who have nothing comforting or original to say.");
	_grumpy_thoughts.push_back("Maybe I'd always been broken and dark inside.");
	_grumpy_thoughts.push_back("People aren't born sad; we make them that way.");
	_grumpy_thoughts.push_back("Some people avoid thinking deeply in public, only because they are afraid of coming across as suicidal.");
	_grumpy_thoughts.push_back("I'm busy saving everybody else when I can't even save myself");
}

void	Happybot::set_as_grumpy( void )
{
	_mood = GRUMPY;
}
void	Happybot::set_as_happy( void )
{
	_mood = HAPPY;
}

string	Happybot::get_random_thought( void )
{
	if (_mood == HAPPY)
		return get_random_happy_thought();
	if (_mood == GRUMPY)
		return get_random_grumpy_thought();
	return string("");
}

string	Happybot::get_random_happy_thought( void )
{
	unsigned long	r_int;

	r_int = static_cast<unsigned long>(rand()) % _happy_thoughts.size();
	return _happy_thoughts[r_int];
}

string	Happybot::get_random_grumpy_thought( void )
{
	unsigned long	r_int;

	r_int = static_cast<unsigned long>(rand()) % _grumpy_thoughts.size();
	return _grumpy_thoughts[r_int];
}

int		Happybot::connect_to_server( string hostname, int port, string password )
{
	int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
	std::string	input;		

    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        printf("\n Socket creation error \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)
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
	_pollfd[0].fd = sock;
	_pollfd[0].events = POLLIN | POLLOUT;
	int res;

	input = "PASS " + password + "\r\n";
	send(_pollfd[0].fd, input.c_str(), input.size(), 0);
	input = "NICK " + _name + "\r\n";
	send(_pollfd[0].fd, input.c_str(), input.size(), 0);
	input = "USER " + std::string("happybot") + " * * :" + "happybot" + "\r\n";
	send(_pollfd[0].fd, input.c_str(), input.size(), 0);

	return 0;
}

int		Happybot::run_bot( void )
{	
	time_t	last_run;
	int		res;
	int		valread = 0;
	int		client_fd = _pollfd->fd;
	char	buffer[1024];
	string	input;	

	//join_happy_world();
	input = "JOIN #happy_world\r\n";
	send(_pollfd[0].fd, input.c_str(), input.size(), MSG_DONTWAIT);
	while(1)
	{
		if (!input.size())
        {
			std::getline(std::cin, input);
            if (!input.compare("EXIT"))
                break;
            input.append("\r\n");
        }
        
		res = poll(_pollfd, 1, 0);
		if (res != 0)
		{
			if ((_pollfd[0].revents & POLLIN) == POLLIN)
			{
				valread = read(_pollfd[0].fd, buffer, 1024);
				if (valread > 0)
					printf("\nRead : %s\n", buffer);
			}
			if (_pollfd[0].revents & POLLOUT && input.size())
			{
				if (send(_pollfd[0].fd, input.c_str(), input.size(), MSG_DONTWAIT))
					input.clear();
				input = "NOTICE #happy_world :" + get_random_happy_thought();
				send(_pollfd[0].fd, input.c_str(), input.size(), MSG_DONTWAIT);
				input.clear();
			}
		}
	}
  
    // closing the connected socket
    close(client_fd);
	return 0;
}

void	Happybot::check_input( const string& input )
{
	size_t	start = 0;
	size_t	next = 0;
	
	t_cmd_function_ptr command;
	while ((next = buffin.find("\r\n", start)) != string::npos)
	{
		msg = Message(client);
		msg.append_in(buffin.substr(start, next - start));
		if (_is_allowed(msg[0], client) == true)
		{
			command = get_command_ptr(msg[0]);
			if (command)
			{
				command(msg);
				client->append_buff(BUFFOUT, msg.get_message_out());
				if (client->is_registered() == false)
					_server->check_registration(client);
			}
		}
		msg.clear_all();
		start = next + 2;
	}
	if (start != string::npos)
		buffin.erase(0, start);
	return ;
}

void	Happybot::join_happy_world( void )
{

}
void	Happybot::send_love_to_everybody( void )
{

}
void	Happybot::send_love_to_someone( string target_nick )
{

}
void	Happybot::send_love_to_channel( string channel_name )
{

}



} // namespace irc end bracket

int main (int argc, char **argv)
{
	irc::Happybot			happybot;
	irc::Happybot::t_info	info;

	if (argc != 4)
		return 1;

	if (!happybot.connect_to_server(argv[1], std::stoi(std::string(argv[2])), argv[3]))
		happybot.run_bot();
	return 0;

}
