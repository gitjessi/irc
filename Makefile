NAME = ./ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address
RM = rm -rf

SRCS =	mandatory/srcs/channel/Channel.cpp \
		mandatory/srcs/clients/Client.cpp \
		mandatory/srcs/commandes/commande_utils.cpp \
		mandatory/srcs/commandes/Commandes.cpp \
		mandatory/srcs/commandes/Invite.cpp \
		mandatory/srcs/commandes/Join.cpp \
		mandatory/srcs/commandes/Kick.cpp \
		mandatory/srcs/commandes/Mode.cpp \
		mandatory/srcs/commandes/Nick.cpp \
		mandatory/srcs/commandes/Pass.cpp \
		mandatory/srcs/commandes/Privmsg.cpp \
		mandatory/srcs/commandes/Topic.cpp \
		mandatory/srcs/commandes/User.cpp \
		mandatory/srcs/server/Server.cpp \
		mandatory/srcs/main.cpp \
		mandatory/srcs/utils.cpp

BONUS =	bonus/srcs/Bot/bot_bonus.cpp \
		bonus/srcs/channel/Channel_bonus.cpp \
		bonus/srcs/clients/Client_bonus.cpp \
		bonus/srcs/commandes/commande_utils_bonus.cpp \
		bonus/srcs/commandes/Commandes_bonus.cpp \
		bonus/srcs/commandes/Invite_bonus.cpp \
		bonus/srcs/commandes/Join_bonus.cpp \
		bonus/srcs/commandes/Kick_bonus.cpp \
		bonus/srcs/commandes/Mode_bonus.cpp \
		bonus/srcs/commandes/Nick_bonus.cpp \
		bonus/srcs/commandes/Pass_bonus.cpp \
		bonus/srcs/commandes/Privmsg_bonus.cpp \
		bonus/srcs/commandes/Topic_bonus.cpp \
		bonus/srcs/commandes/User_bonus.cpp \
		bonus/srcs/server/Server_bonus.cpp \
		bonus/srcs/main_bonus.cpp \
		bonus/srcs/utils_bonus.cpp

OBJ = $(SRCS:.cpp=.o)
BONUS_OBJ = $(BONUS:.cpp=.o)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo Compiled

bonus: $(BONUS_OBJ)
	@$(CC) $(FLAGS) $(BONUS_OBJ) -o $(NAME)_bonus
	@echo Compiled

%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

clean:
	@$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	@$(RM) $(NAME) $(NAME)_bonus
	@echo Cleaned

re: fclean all

.PHONY: all clean fclean re bonus
