NAME		=	bot

CC			=	c++ -std=c++98
CFLAGS		=	-Wall -Werror -Wextra

SRC_DIR		=	./src
OBJ_DIR		=	./obj


SRC			=	Client.cpp main.cpp

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

all			:	$(NAME)

$(OBJ)		: 	| $(OBJ_DIR)

$(OBJ_DIR)	:
				@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp
					$(CC) $(CFLAGS) -c $< -o $@

$(NAME)		:	$(OBJ)
				$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean		:
				@$(RM_DIR) $(OBJ_DIR)

fclean		:	clean
				@$(RM_FILE) $(NAME)

re			:	fclean all

.PHONY		:	all, clean, fclean, re