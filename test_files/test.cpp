/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 17:04:37 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/02 09:05:19 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "commands.hpp"
#include "typedef.hpp"

using namespace irc;
using std::cout;
using std::cerr;
using std::endl;

void	process_message( Message& msg )
{
	Server & server  = Server::get_server();
	t_cmd_function_ptr command;
	
	command = server.get_command_ptr(msg[0]);
	if (command)
		command(msg);
	else
		cout << "command " << msg[0] << " not found" << endl;
	cout << msg.get_message_out();
}

int main ( void )
{
	Server & server  = Server::get_server();
	t_addr6 addr;

	server.add_client(Client("Tok"));
	server.add_client(Client("Seb"));
	server.add_client(Client("Alex"));
	server.add_client(Client("Justine_la_pas_ignoree"));
	cout << "client count : " << server.get_client_count() << endl;
	cout << "client in list : " << server.get_client("Tok")->get_nickname() << endl;
	server.remove_client("Seb");
	cout << "deleting Seb" << endl;
	cout << "client count : " << server.get_client_count() << endl;

	Message		msg = Message(server.get_client("Justine_la_pas_ignoree"));
	
	cout << "client justine : " << server.get_client("Justine_la_pas_ignoree")->get_nickname() << endl;
	msg.append_in("NICK Tok");
	process_message(msg);
	// cout << endl << "client justine : " << server.get_client("Justine")->get_nickname() << endl;

	return 0;
}
