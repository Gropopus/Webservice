# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/24 15:16:37 by gmaris            #+#    #+#              #
#    Updated: 2021/12/16 18:57:11 by thsembel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = clang++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address

SRCS_DIR = ./srcs/

SRC 		=	Webserv.cpp\
				Env.cpp\
				FileParsing.cpp\
				Server.cpp \
				Client.cpp \
				Parse_Request.cpp \
				HandleRequest.cpp \
				Utils.cpp\
				Post_handler.cpp \
				AutoIndex.cpp \
				Chunkistan.cpp \
				ErrorPages.cpp

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

PWD			=	$(shell pwd)
YELLOW = \033[033m
GREEN = \033[032m
BLUE = \033[36m
RED = \033[031m
PURPLE = \033[35m
RESET = \033[0m

$(OBJS_DIR)%.o :	$(SRCS_DIR)%.cpp $(INCS)
	@mkdir -p $(OBJS_DIR)
	@echo "$(YELLOW)$(notdir $(basename $@))...$(RESET)"
	@$(CC) -I./includes $(FLAGS) -c $< -o $@
	@echo "\t\t[$(GREEN)OK$(RESET)]"

$(NAME): $(OBJS) Makefile $(INCS) ./conf/correct.conf
	@$(CC) $(INC_FLAG) $(FLAGS) -o $(NAME) $(OBJS)
	@echo "[$(GREEN)$(NAME) compiled$(RESET)]"

./conf/correct.conf:
	@cat ./conf/testy.conf | sed "s=PWD=$(PWD)=g" > ./conf/correct.conf

all: $(NAME)

bonus:	$(NAME)

clean:
	@echo "Cleaning: $(OBJS_DIR)"
	@rm -rf $(OBJS_DIR)

fclean: clean
	@echo "Cleaning: $(NAME) && correct.conf"
	@rm -f $(NAME)
	@rm -f ./conf/correct.conf

re: fclean all

.PHONY: fclean clean re all
