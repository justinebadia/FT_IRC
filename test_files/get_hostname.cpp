#include <netdb.h>
#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(void)
{
	struct hostent*	host;
	char **aliases;

	host = gethostbyname("127.0.0.1");
	aliases = host->h_aliases;
	cout << "Name: " << host->h_name << endl;
	cout << "Aliases: ";
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr type: " << host->h_addrtype << endl;
	cout << "Addr len: " << host->h_length << endl;
	cout << "Addr list: ";
	aliases = host->h_addr_list;
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Aliases: ";
	cout << "Addr: " << host->h_addr << endl;
	cout << endl;
	cout << endl;

	host = gethostbyname("localhost");
	aliases = host->h_aliases;
	cout << "Name: " << host->h_name << endl;
	cout << "Aliases: ";
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr type: " << host->h_addrtype << endl;
	cout << "Addr len: " << host->h_length << endl;
	cout << "Addr list: ";
	aliases = host->h_addr_list;
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Aliases: ";
	cout << "Addr: " << host->h_addr << endl;
	cout << endl;
	cout << endl;

	host = gethostbyname("10.11.7.4");
	aliases = host->h_aliases;
	cout << "Name: " << host->h_name << endl;
	cout << "Aliases: ";
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr type: " << host->h_addrtype << endl;
	cout << "Addr len: " << host->h_length << endl;
	cout << "Addr list: ";
	aliases = host->h_addr_list;
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr: " << host->h_addr << endl;
	cout << endl;
	cout << endl;

	host = gethostbyname("localhost");
	aliases = host->h_aliases;
	cout << "Name: " << host->h_name << endl;
	cout << "Aliases: ";
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr type: " << host->h_addrtype << endl;
	cout << "Addr len: " << host->h_length << endl;
	cout << "Addr list: ";
	aliases = host->h_addr_list;
	while (*aliases)
		cout << string(*(aliases++)) << " ";
	cout << endl;
	cout << "Addr: " << host->h_addr << endl;
	cout << endl;
	cout << endl;
}