
# Compilation
CC		= clang++
CFLAGS	=  -g  -Wall -Wextra -Wshadow -Wconversion -Wpedantic -std=c++98
C_OBJ	= $(CC) $(CFLAGS) $(INC) -c $< -o $@
C_MAIN	= $(CC) $(CFLAGS) $(INC) $(OBJ) $(MAIN) -o $(NAME)

# Program
NAME	= ircserv
DESC	= \"ft_irc : coding our own IRC server\"

# Directories
D_SRC	= src
D_INC	= include
D_OBJ	= obj


# Files
MAIN	= test.cpp

_HEAD	= vector.hpp iterator.hpp iterator_traits.hpp vector_iterator.hpp enable_if.hpp test.hpp
HEAD	= $(_HEAD)

_SRC	= test_pair.cpp
SRC		= $(patsubst %.cpp, $(D_SRC)/%.cpp, $(_SRC))

_OBJ	= $(_SRC:.cpp=.o)
OBJ		= $(patsubst %.o, $(D_OBJ)/%.o, $(_OBJ))

$(D_OBJ)/%.o : %.cpp
		$(C_OBJ)

# Recipes
all		: $(NAME)

$(NAME)	: $(HEAD) $(SRC) $(D_OBJ) $(OBJ) $(MAIN)
		$(C_MAIN)
		# $(shell echo "Compiling $(P_NAME) done!")
		# $(shell echo "Executable is : $(NAME)")

$(D_OBJ)	:
		@ mkdir $(D_OBJ)

clean	: 
		@ rm -rf $(D_OBJ)
		# $(shell echo "Cleaning done!")

fclean	: clean
		@ rm -f $(NAME)

re		: fclean all

exe		: test
		# $(shell ./$(NAME))

test	: _test $(NAME)

_test	:
		$(eval CFLAGS= -Wall -Wextra -Wshadow -Wconversion -Wpedantic -std=c++98)

.PHONY	: all clean fclean re test exe