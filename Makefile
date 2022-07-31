
# Compilation
CC		= clang++
CFLAGS	= -Werror -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Iinclude -std=c++98
C_OBJS	= $(CC) $(CFLAGS) $(INCS) -c $< -o $@
C_MAIN	= $(CC) $(CFLAGS) $(INCS) $(OBJS) $(MAIN) -o $(NAME)

# Program
EXE		= ircserv
NAME	= ircserv
DESC	= \"ft_irc : coding our own IRC server\"

# Directories
D_SRCS	= srcs
D_INCS	= includes
D_OBJS	= obj # not used

INCS	= -I$(D_INCS)

# Files
MAIN		= 	test_files/test.cpp

_CLASS_SRCS	=	Client/Client.cpp
				
CLASS_SRCS	=	$(patsubst %.cpp, $(D_SRCS)/%.cpp, $(_CLASS_SRCS))

_CLASS_HDRS	=	Client/Client.hpp \
				Server/Server.hpp
				
CLASS_HDRS	= $(patsubst %.hpp, $(D_SRCS)/%.hpp, $(_CLASS_HDRS))

_UTILS_SRCS	=	
_UTILS_HDRS =	color.hpp \
				irc_define.hpp \
				numeric_replies.hpp \
				struct.hpp \
				typedef.hpp
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