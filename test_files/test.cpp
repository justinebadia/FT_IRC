/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfournie <sfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 17:04:37 by sfournie          #+#    #+#             */
/*   Updated: 2022/07/31 18:12:47 by sfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/Client/Client.hpp"

using std::cout;
using std::cerr;
using std::endl;

int main ( void )
{
	t_addr6 addr;
	Client	client(1, addr);

	client.set_nickname("Tok");
	cout << "client in list : " << client.get_nickname();
	cout << client << endl;
	cout << "client count : " << Client::get_client_count() << endl;
	cout << "list size : " << Client::get_client_list().size() << endl;
	cout << "client in list : " << client.get_nickname();
	cout << "client in list : " << Client::get_client("Tok")->get_nickname();

	return 0;
}
