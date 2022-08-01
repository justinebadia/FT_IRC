/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 17:04:37 by sfournie          #+#    #+#             */
/*   Updated: 2022/08/01 12:44:22 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/Client/Client.hpp"
#include "typedef.hpp"
#include "../srcs/Server/Server.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main ( void )
{
	Server & server  = Server::get_server();
	t_addr6 addr;
	t_client_list client_list;

	server.add_client(Client("Tok"));
	server.add_client(Client("Seb"));
	server.add_client(Client("Alex"));
	cout << "client count : " << server.get_client_count() << endl;
	cout << "client in list : " << server.get_client("Tok")->get_nickname() << endl;
	server.remove_client("Seb");
	cout << "deleting Seb" << endl;
	cout << "client count : " << server.get_client_count() << endl;

	return 0;
}
