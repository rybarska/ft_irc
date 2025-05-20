SRC = ./main.cpp \
      ./Server.cpp \
      ./Client.cpp \

OBJ = $(SRC:.cpp=.o)
CXX = c++
CXXFLAGS = -Werror -Wextra -Wall -std=c++98 -MMD -MP
NAME = ircserv

all:$(NAME)

$(NAME):$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
	rm -f $(OBJ:.o=.d)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(OBJ:.o=.d)

.PHONY: all clean fclean re
