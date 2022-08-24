
#include <iostream>
#include <regex>
#include <string>
#include <list>
#include "Server.hpp"


#include "utils.hpp"

using std::string;
using std::cerr;
using std::endl;
using std::regex;
using std::list;

namespace irc
{

bool	validate_with_regex( string regex_format, string entry)
{
	regex	reg_cmp(regex_format);
	bool res = std::regex_match(entry, reg_cmp);
	return res;
}

bool	compare_to_mask_list( list<string>* mask_list, const string& str )
{
	list<string>::iterator	it;

	for(it = mask_list->begin(); it != mask_list->end(); it++)
	{
		if (compare_to_mask(*it, str))
			return true; 
	}
	return false;
}

bool	compare_to_mask( const string& mask, const string& str )
{
	size_t	mask_i = 0;
	size_t	str_i = 0;
	size_t	next_wild;
	size_t	find_mask;
	size_t	sublen;

	if (mask[mask_i] != '*' && mask[mask_i] != str[str_i])
		return false;
	if (mask[mask.length() - 1] != '*' && mask[mask.length() - 1] != str[str.length() - 1])
		return false;

	while (mask[mask_i])
	{
		while (mask[mask_i] == '*')
			mask_i++;

		if (!mask[mask_i])
			return true;

		// Accept any one character if (?) is found. 
		if (mask[mask_i] == '?')
		{
			mask_i++;
			if (!str[str_i++] && !mask[mask_i])
				return false;
			continue;
		}
		// Get the index of the next (*)
		next_wild = mask.find("*", mask_i);
		if (next_wild == string::npos)
			next_wild = mask.length() - 1;
		sublen = next_wild - mask_i;

		// Attempt to find the mask substr
		find_mask = str.find(mask.substr(mask_i, sublen), str_i);
		if (find_mask == string::npos)
			return false;
		
		mask_i += sublen + 1;
		str_i = find_mask + sublen;
	}
	return true;
}

string	convert_ip_address(const string &ip) // convert IPv4 mapped as IPv6 to IPv4 notation
{
	if (ip.compare(0, 7, "::ffff:") == 0 || ip.compare(0, 7, "::FFFF:") == 0)
		return ip.substr(7);
	else
		return ip;  
}

} // namespace irc end bracket

