NAME		=	IRC

CC			=	c++ -std=c++98
CFLAGS		=	-Wall -Werror -Wextra

SRC_DIR		=	./src
OBJ_DIR		=	./obj

SRC			=	Channel.cpp Server.cpp Utils.cpp irc_operatror_commands.cpp registration_commands.cpp \
                main.cpp send_error.cpp Message.cpp User.cpp channel_commands.cpp send_reply.cpp \
                init_commands.cpp other_commands.cpp user_commands.cpp

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

BOT_DIR		= bot/

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

all			:	$(NAME) bonus

bonus		:
	$(MAKE) -C $(BOT_DIR) all

$(OBJ)		: 	| $(OBJ_DIR)

$(OBJ_DIR)	:
				mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp
					$(CC) $(CFLAGS) -c $< -o $@

$(NAME)		:	$(OBJ)
				$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean		:
				$(RM_DIR) $(OBJ_DIR)
				$(MAKE) -C $(BOT_DIR) clean

fclean		:	clean
				$(RM_FILE) $(NAME)
				$(MAKE) -C $(BOT_DIR) fclean

re			:	fclean all

.PHONY		:	all, clean, fclean, re, bonus

