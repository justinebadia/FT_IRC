# **FT_IRC 42 school project** _- with [Tokutaro](https://github.com/Rurikide) & [Sébastien](https://github.com/Fousse24)_

This project is about creating our own IRC server.
<img width="625" alt="Capture d’écran, le 2022-08-24 à 15 52 58" src="https://user-images.githubusercontent.com/79991066/186510643-fbe32350-56a9-4864-a12d-6cca3a9ae882.png">

> **I**nternet **R**elay **C**hat (**IRC**) is a text-based chat system for instant messaging; designed for group communication in channels, but also for one-to-one communication via private messages.

## How to ?
Clone the repo :
```
git clone https://github.com/justinebadia/FT_IRC.git
```

Do `make`and then run :
```
./ircserv 6667 pass
```
Our server listen for incoming connections on port 6667. The second argument is the password that each client need to connect.

# With netcat 
To ensure that our server correctly process everything that you send to it, you can use netcat command. 
The basic syntax for nc command is : 
``` nc [<options>] <host> <port> ```

So, you can type:
```
nc -C 127.0.0.1 6667
```

The -C option send CRLF as line-ending. You can also type `control+v` followed by `control+m` to insert a carriage-return.

First, you have to register as a user, so you'll need to enter the correct password, and the user and nick commands.
``` 
PASS pass
USER user * * :realname
NICK nickname 
```
Once you're register, you'll receive a welcome reply : 
`Welcome to the Internet Relay Network nickname!user@127.0.0.1`

# With Hexchat 


## Commands 
| Commands | Description |
| :---: | --- |
| **_Connection registration_**  |  |
| PASS \<password\> |  - *Must be send before the NICK and USER commands.*|
| NICK | NICK \<nickname\> - *Allows a client to change their IRC nickname.*|
| USER | USER \<user\> \<mode\> \<unused\> \<realname\> - *Is used at the beginning of a connection.*|
| OPER | OPER \<username\> \<password\> - *Authenticates a user as an IRC operator on that server.*|
| QUIT | QUIT [\<message\>] - *Disconnects the user from the server.*|
| SQUIT | SQUIT \<server\> \<comment\> - *Causes \<server\> to quit the network.*|
| **_Channel operations_** | |
| JOIN | JOIN \<channels\> [\<keys\>] - *Makes the client join the channels in the comma-separated list \<channels\>, specifying the passwords, if needed, in the comma-separated list \<keys\>. If the channel(s) do not exist then they will be created.*|
| PART | PART \<channels\> [\<message\>] - *Causes a user to leave the channels in the comma-separated list \<channels\>.*|
| MODE | MODE \<channel\> \<flags\> [\<args\>] - *Used to set channel modes.*|
| TOPIC | TOPIC <channel> [<topic>] - *Allows the client to query or set the channel topic on \<channel\>. If \<topic\> is given, it sets the channel topic to \<topic\>.*|
| NAMES | NAMES [\<channels\>] - *Returns a list of who is on the comma-separated list of \<channels\>, by channel name. If \<channels\> is omitted, all users are shown.*|
| LIST | LIST \<channels\> - *Lists all channels on the server.*|
| INVITE |INVITE \<nickname>\ \<channel\> - *Invites \<nickname\> to the channel \<channel\>. On our server, \<channel\> has to be in mode +i, so only channel operators may invite other clients.*|
| KICK | KICK \<channel\> \<client\> :[\<message\>] - *This command may only be issued by channel operators. It forcibly removes \<client\> from \<channel\>.*|
| **_Sending messages_** | |
| PRIVMSG | PRIVMSG \<target\> :\<message\> - *Sends \<message\> to \<msgtarget\>, which is usually a user or channel.*|
| NOTICE | NOTICE \<target\> \<message\> - *Works similarly to PRIVMSG, except automatic replies must never be sent in reply to NOTICE messages.*|
| **_User queries_** | |
| WHO | WHO \<name\> - *Returns a list of users who match \<nickname\>.*|
| WHOIS | WHOIS \<nickname\> - *Returns information about \<nickname\>.*|
| **_Miscellaneous messages_** | |
| KILL | KILL <client> <comment> - *This command may only be issued by IRC operators. It forcibly removes \<client\> from the network.*|
| PING | PING - *Tests the presence of a connection. PING message results in a PONG reply.*|
| ERROR | ERROR <error message> - *It is used before terminating client connections.*|
