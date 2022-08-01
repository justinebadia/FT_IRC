
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
D_MSG	:= $(D_SRCS)/Message
D_CHAN	:= $(D_SRCS)/Channel
D_OBJS	:= obj # not used

INCS	= -I$(D_INCS) -I$(D_CLI) -I$(D_SERV) -I$(D_MSG) -I$(D_CHAN)

# Files
MAIN		= 	test_files/test.cpp

_CLASS_SRCS	=	Client/Client.cpp \
				Message/Message.cpp \
				Server/Server.cpp 
				# Channel/Channel.cpp
				
CLASS_SRCS	=	$(patsubst %.cpp, $(D_SRCS)/%.cpp, $(_CLASS_SRCS))

_CLASS_HDRS	=	Client/Client.hpp \
				Server/Server.hpp \
				Message/Message.hpp 
				# Channel/Channel.hpp
				
CLASS_HDRS	= $(patsubst %.hpp, $(D_SRCS)/%.hpp, $(_CLASS_HDRS))

_UTILS_SRCS	=	commands.cpp \
				num_replies.cpp \
				utils.cpp
UTILS_SRCS	=	$(patsubst %.cpp, $(D_SRCS)/%.cpp, $(_UTILS_SRCS))

_UTILS_HDRS =	color.hpp \
				commands.hpp \
				irc_define.hpp \
				numeric_replies.hpp \
				typedef.hpp \
				utils.hpp 

UTILS_HDRS	= $(patsubst %.hpp, $(D_INCS)/%.hpp, $(_UTILS_HDRS))

SRCS		= $(CLASS_SRCS) $(UTILS_SRCS)
HDRS		= $(CLASS_HDRS) $(UTILS_HDRS)
OBJS		= $(SRCS:.cpp=.o)

%.o : %.cpp
		$(C_OBJS)

# Recipes
all		: $(NAME)

$(NAME)	: $(HDRS) $(SRCS) $(OBJS) $(MAIN)
		$(C_MAIN)
		# $(shell echo "Compiling $(EXE) done!")
		# $(shell echo "Executable is : $(NAME)")

$(D_OBJS)	:
		@ mkdir $(D_OBJS)

clean	: 
		@ rm -rf $(OBJS)
		# $(shell echo "Cleaning done!")

fclean	: clean
		@ rm -f $(NAME)

re		: fclean all

exe		: test
		# $(shell ./$(NAME))

test	: _test $(NAME)

_test	:
		$(eval CFLAGS= -g -Wall -Wextra -Wshadow -Wconversion -Wpedantic -std=c++98)

.PHONY	: all clean fclean re test exe