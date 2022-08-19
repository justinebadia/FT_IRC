
# Compilation
CC		= clang++
CFLAGS	= -Werror -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Iinclude -std=c++98
C_OBJS	= $(CC) $(CFLAGS) $(INCS) -c $< -o $@
C_MAIN	= $(CC) $(CFLAGS) $(INCS) $(OBJS) $(MAIN) -o $(NAME)

# Program
EXE		= ircserv
NAME	= ircserv
DESC	= \"ft_irc : coding our own IRC server\"

MAKE_PATH	 := $(abspath $(lastword $(MAKEFILE_LIST)))


# Directories
D_MAKE	:= $(dir $(MAKE_PATH))
D_SRCS	:= $(D_MAKE)srcs
D_INCS	:= $(D_MAKE)includes
D_CLI	:= $(D_SRCS)/Client
D_SERV	:= $(D_SRCS)/Server
D_CMD	:= $(D_SRCS)/Command
D_CMDS	:= $(D_CMD)/cmd_srcs
D_CHAN	:= $(D_SRCS)/Channel
D_DB	:= $(D_SRCS)/Database
D_OBJS	:= obj # not used

INCS	= -I$(D_INCS) -I$(D_CLI) -I$(D_SERV) -I$(D_CMD) -I$(D_CHAN) -I$(D_DB)

# Files
MAIN		= 	test_files/main_loop.cpp

_CLASS_SRCS	=	$(D_CHAN)/Channel.cpp \
				$(D_CLI)/Client.cpp \
				$(D_DB)/Database.cpp \
				$(D_CMD)/Message.cpp \
				$(D_CMD)/CommandManager.cpp \
				$(D_SERV)/Server.cpp
				
CLASS_SRCS	=	$(patsubst %.cpp, %.cpp, $(_CLASS_SRCS))

_CMDS_SRCS	=	$(D_CMD)/commands.cpp \
				$(D_CMD)/utils_commands.cpp \
				$(D_CMD)/num_replies.cpp \
				$(D_CMDS)/error.cpp \
				$(D_CMDS)/join.cpp \
				$(D_CMDS)/list.cpp \
				$(D_CMDS)/mode.cpp \
				$(D_CMDS)/names.cpp \
				$(D_CMDS)/part.cpp \
				$(D_CMDS)/privmsg.cpp \
				$(D_CMDS)/WHO.cpp 
				

CMDS_SRCS	=	$(patsubst %.cpp, %.cpp, $(_CMDS_SRCS))

_CLASS_HDRS	=	$(D_CHAN)/Channel.hpp \
				$(D_CLI)/Client.hpp \
				$(D_DB)/Database.hpp \
				$(D_CMD)/Message.hpp \
				$(D_CMD)/CommandManager.hpp \
				$(D_SERV)/Server.hpp
				


CLASS_HDRS	= $(patsubst %.hpp, %.hpp, $(_CLASS_HDRS))

_UTILS_SRCS	=	utils.cpp

UTILS_SRCS	=	$(patsubst %.cpp, $(D_SRCS)/%.cpp, $(_UTILS_SRCS))

_UTILS_HDRS =	color.hpp \
				irc_define.hpp \
				numeric_replies.hpp \
				typedef.hpp \
				utils.hpp 

UTILS_HDRS	= $(patsubst %.hpp, $(D_INCS)/%.hpp, $(_UTILS_HDRS))

SRCS		= $(CLASS_SRCS) $(CMDS_SRCS) $(UTILS_SRCS)
HDRS		= $(CLASS_HDRS) $(UTILS_HDRS)
OBJS		= $(SRCS:.cpp=.o)

PRINTED :=
%.o : %.cpp
		@ $(or $(PRINTED),$(eval PRINTED := :)/bin/echo -n "Compiling object files")
		@ /bin/echo -n "."
		@ $(C_OBJS)

# Recipes
all		: $(NAME)

$(NAME)	: $(HDRS) $(SRCS) $(OBJS) $(MAIN)
		@ $(C_MAIN)
		@ /bin/echo Compiling done!
		@ /bin/echo Executable is : $(NAME)

$(D_OBJS)	:
		@ mkdir $(D_OBJS)

clean	: 
		@ rm -rf $(OBJS)
		# $(shell echo "Cleaning done!")

fclean	: clean
		@ rm -f $(NAME)

re		: fclean all

server	: test
		# $(shell ./$(NAME) 6667 pass)

client	: 
		# $(shell clang++ main_client.cpp -o client ; ./client)

test	: _test $(NAME)

_test	:
		$(eval CFLAGS= -g -Wall -Wextra -Wshadow -Wconversion -Wpedantic)

.PHONY	: all clean fclean re test server client