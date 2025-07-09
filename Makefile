SRC = ./main.cpp \
      ./Server.cpp \
      ./Client.cpp \
      ./Message.cpp \
      ./CommandControl.cpp \
	  ./Channel.cpp \
	  ./IRCReplies.cpp

OBJ = $(SRC:.cpp=.o)

BOT_SRC = ./bot/main.cpp ./bot/Bot.cpp
BOT_OBJ = $(BOT_SRC:.cpp=.o)

CXX = c++
CXXFLAGS = -Werror -Wextra -Wall -std=c++98 -MMD -MP

NAME = ircserv
BOT_NAME = ircbot

all:$(NAME)

$(NAME):$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

bot: $(BOT_OBJ)
	$(CXX) $(CXXFLAGS) $(BOT_OBJ) -o $(BOT_NAME)
clean:
	rm -f $(OBJ) $(BOT_OBJ)
	rm -f $(OBJ:.o=.d) $(BOT_OBJ:.o=.d)

fclean: clean
	rm -f $(NAME) $(BOT_NAME)

re: fclean all bot

-include $(OBJ:.o=.d)
-include $(BOT_OBJ:.o=.d)

.PHONY: all clean fclean re bot
