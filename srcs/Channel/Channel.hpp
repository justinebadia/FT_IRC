#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
// #include <list>
// #include <map>
#include <string>
#include "Client.hpp"
#include "typedef.hpp"

#define	REGEX_CHANNEL		"^[#$]((?![ ,/\x07/]).){1,50}$"

using std::string;
using std::list;
using std::pair;

/*============================================================================*/ 
namespace irc {

class Channel {

public:

	/*--------------------TYPEDEF-&-ENUM-------------------*/
	typedef	t_channel_memberlist::iterator	iterator;

	enum mode_flags
	{
		FLAG_O = 1,
		FLAG_I = 2,
		FLAG_T = 4,
		FLAG_K = 8,
		FLAG_B = 16
	};

private:

	/*---------------PROHIBITED-CONSTRUCTORS--------------*/


	/*---------------------ATTRIBUTES---------------------*/

	string								_name;						// channel_name
	Client*								_owner;
	// bool								_private;					// ±p
	// bool								_secret;					// ±s
	bool								_invite_only;				// ±i
	bool								_password_required;			// ±flag pour +k
	bool								_topic_by_chanop_only;		// ±t
	string								_topic;
	string								_password;
	t_channel_memberlist				_memberlist;
	int									_mode_flags;
	string								_mode_str;
	t_mask_list							_banmask_list;


public:

	/*--------------CONSTRUCTORS-&-DESTRUCTOR-------------*/
	
	Channel( void ); 																				// default constructor
	Channel( const string& channel_name, Client* channel_owner ); 									// main constructor
	// Channel( const string& channel_name, Client* channel_owner, const string& channel_password );	// password required constructor
	Channel( const Channel& rhs );																	// copy constructor
	~Channel( void );																				// destructor
	Channel& operator=( const Channel& rhs );	 													// copy operator overload


	/*---------------OTHER-OPERATOR-OVERLOAD--------------*/

	bool	operator==( const Channel& rhs) const;


	/*-----------------------GETTERS----------------------*/

	const string&						get_name( void ) const;
	Client*								get_owner( void ) const;
	// bool								get_is_private( void ) const;
	// bool								get_is_secret( void ) const;
	bool								get_is_invite_only( void ) const;
	bool								get_is_password_required( void ) const;
	bool								get_is_topic_by_chanop_only( void ) const;
	string&								get_topic( void );
	string&								get_password( void );
	t_channel_memberlist				get_memberlist( void );
	int 								get_mode_flags( void );
	string&								get_mode_str( void );
	e_permission						get_permission( Client* client );
	t_client_ptr_list					get_clients_any_permissions( void );
	t_client_ptr_list					get_clients_matching_permissions( int type );
	t_client_ptr_list					get_clients_not_matching_permissions( int type );
	t_client_ptr_list					get_clients_not_banned( void );
	t_mask_list							get_banmask_list( void );



	/*-----------------------SETTERS----------------------*/

	void	set_channel_owner( Client* new_owner );
	int		set_permission( Client* client, e_permission type );
	void	set_mode_private( bool setting );
	void	set_mode_secret( bool setting );
	void	set_mode_invite_only( bool setting );
	void	set_mode_key_password_required( bool setting );
	void	set_mode_topic_by_chanop_only( bool setting );

	void	set_topic( const string& topic );
	void	set_password( const string& password );
	void	set_mode_str( const string& flag );


	/*---------------OTHER-MEMBER-FUNCTIONS---------------*/

	bool	is_member( Client* client );
	bool	is_owner( Client* client );
	bool	is_chanop( Client* client );
	bool	is_banned( Client* client );
	bool	is_banned( const string& nickname );
	bool	is_only_banned_member_left( void );
	bool	is_empty( void );

	void	add_member( Client* client, e_permission type );
	void	remove_member( Client* client );
	void	print_memberlist( void );
	void	empty_memberlist( void );
	void	add_banmask( const string& mask );
	void	remove_banmask( const string& mask );

	void	transfer_ownership( void );
	string	parse_modes( Message& msg );


};

	/*-----------------NON-MEMBER-FUNCTIONS---------------*/

	std::ostream&	operator<<( std::ostream& o, const Channel& obj );


} // namespace irc end bracket

#endif
