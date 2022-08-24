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
| **_Connection registration_**  | --- |
| PASS | ---|
| NICK | ---|
| USER | ---|
| OPER | ---|
| QUIT | ---|
| SQUIT | ---|
| **_Channel operations_** | ---|
| JOIN | ---|
| PART | ---|
| MODE | ---|
| TOPIC | ---|
| NAMES | ---|
| LIST | ---|
| INVITE | ---|
| KICK | ---|
| **_Sending messages_** | ---|
| PRIVMSG | ---|
| NOTICE | ---|
| **_User queries_** | ---|
| WHO | ---|
| WHOIS | ---|
| **_Miscellaneous messages_** | ---|
| KILL | ---|
| PING | ---|
| ERROR | ---|
