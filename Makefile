# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/24 15:16:37 by gmaris            #+#    #+#              #
#    Updated: 2021/11/25 15:58:54 by thsembel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = clang++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

SRCS_DIR = ./srcs/
SRC 		=	Webserv.cpp\
				Env.cpp\
				FileParsing.cpp\
				Server.cpp \
				Client.cpp \
				Utils.cpp



INC			=	Web_serv.hpp\
				Env.hpp\
				Server.hpp \
				Client.hpp


OBJS_DIR 	= 	./obj/
OBJ 		= 	$(SRC:.cpp=.o)
OBJS 		= 	$(addprefix $(OBJS_DIR), $(OBJ))

INC_DIR		= 	./includes/
INCS		= 	$(addprefix $(INC_DIR), $(INC))
INC_FLAG 	= 	-I $(INC_DIR)
SRCS 		= 	$(addprefix $(SRCS_DIR), $(SRC))

YELLOW = \033[033m
GREEN = \033[032m
BLUE = \033[36m
RED = \033[031m
PURPLE = \033[35m
RESET = \033[0m

$(OBJS_DIR)%.o :	$(SRCS_DIR)%.cpp $(INCS)
	@mkdir -p $(OBJS_DIR)
	@echo "$(YELLOW)$(notdir $(basename $@))...$(RESET)\t\t[$(GREEN)OK$(RESET)]"
	@$(CC) -I./includes $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) Makefile $(INCS)
	@$(CC) $(INC_FLAG) $(FLAGS) -o $(NAME) $(OBJS)
	@echo "[$(GREEN)$(NAME) compiled$(RESET)]"

all: $(NAME)

clean:
	@echo "Cleaning: $(OBJS_DIR)"
	@rm -rf $(OBJS_DIR)

fclean: clean
	@echo "Cleaning: $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: fclean clean re all
