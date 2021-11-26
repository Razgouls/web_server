# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elie <elie@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/16 07:45:16 by eoliveir          #+#    #+#              #
#    Updated: 2021/11/25 15:00:34 by elie             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[01;32m
YELLOW = \033[01;33m
CYAN = \033[01;36m
RESET = \033[00m
RED = \033[0;31m

NAME = web_server

CC = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98

INC= -I ./includes/cgi -I ./includes/parser -I ./includes/utils -I ./includes/web_server

#############################################
######			FILES .C			#########
#############################################

SRC = srcs

DIR_C = $(addprefix $(SRC)/, cgi parser utils web_server)

FILES_C = $(addprefix $(SRC)/, $(CGI) $(PARSER) $(UTILS) $(WEB_SERVER))

CGI			= $(addprefix cgi/,					\
				CGI.cpp							\
			)

PARSER		= $(addprefix parser/,				\
				main.cpp						\
				ParserServer.cpp				\
				ParserRoute.cpp					\
			)

UTILS		= $(addprefix utils/,				\
				utils.cpp						\
			)

WEB_SERVER	= $(addprefix web_server/,			\
				Request.cpp						\
				Response.cpp					\
				Route.cpp						\
				Server.cpp						\
				ConnexionServer.cpp				\
			)

#############################################
######			FILES .O			#########
#############################################

OBJ_DIR = objs
O_DIR = $(DIR_C:$(SRC)/%=$(OBJ_DIR)/%)
OBJS = $(FILES_C:$(SRC)/%.c=$(OBJ_DIR)/%.o)

#############################################
######			FILES .O			#########
#############################################

#@printf "$(YELLOW)Compiling Object...$(RESET) %33.33s        \r" "$@"

all: $(NAME)

$(NAME): $(OBJS) 
	@printf "$(GREEN)================================================$(RESET)"
	@printf "\n$(GREEN)================== WEB_SERVER ==================$(RESET)"
	@printf "\n$(GREEN)================================================$(RESET)"
	@printf "\n$(YELLOW)Compiling Object...$(RESET)"
	@printf "\n$(GREEN)-->[OK]                       $(RESET)"
	@printf "\n$(CYAN)Compiling $@..."
	-@$(CC) $(FLAGS) $(INC) $(OBJS) -o $(NAME)
	@echo "\n$(GREEN)-->[OK]$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRC)/%.c
	@mkdir -p objs/ $(O_DIR)
	@$(CC) $(FLAGS) -o $@ -c $< $(INC)

clean:
	@printf "$(GREEN)================================================$(RESET)\n"
	@printf "$(GREEN)==================== CLEAR =====================$(RESET)\n"
	@printf "$(GREEN)================================================$(RESET)\n"
	@printf "$(YELLOW)Clear Objects...$(RESET)                                                       "
	@printf "\n$(GREEN)-->[OK]$(RESET)\n"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@printf "$(YELLOW)Remove Executable...$(RESET)"
	@printf "\n$(GREEN)-->[OK] : $(NAME) removed$(RESET)\n\n"

re: fclean all

.SILENT: clean

.PHONY: all clean fclean re
