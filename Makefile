# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elie <elie@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/16 07:45:16 by eoliveir          #+#    #+#              #
#    Updated: 2021/12/05 19:30:21 by elie             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[01;32m
YELLOW = \033[01;33m
CYAN = \033[01;36m
RESET = \033[00m
RED = \033[0;31m

NAME = webserv
NAME_TESTER = web_test

CC = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

INC= -I ./includes/cgi -I ./includes/parser -I ./includes/utils -I ./includes/web_server
INC_TESTER= -I ./tester/include_tester

#############################################
######			FILES .C			#########
#############################################

SRC = srcs
SRC_TESTER = tester

DIR_C = $(addprefix $(SRC)/, cgi parser utils web_server)
DIR_TESTER = $(addprefix $(SRC_TESTER)/, utils_tester delete get headers post)

FILES_C = $(addprefix $(SRC)/, $(CGI) $(PARSER) $(UTILS) $(WEB_SERVER))
FILES_TESTER = $(addprefix $(SRC_TESTER)/, $(UTILS_TESTER) $(HEADERS) $(DELETE) $(GET) $(POST))

CGI			= $(addprefix cgi/,					\
				CGI.cpp							\
			)
PARSER		= $(addprefix parser/,				\
				main.cpp						\
				ParserServer.cpp				\
				ParserRoute.cpp					\
			)
UTILS		= $(addprefix utils/,				\
				Utils.cpp						\
			)
WEB_SERVER	= $(addprefix web_server/,			\
				Request.cpp						\
				Response.cpp					\
				Route.cpp						\
				Server.cpp						\
				ConnexionServer.cpp				\
			)

UTILS_TESTER	= $(addprefix utils_tester/,	\
				client.cpp						\
				Requete_tester.cpp				\
				Utils_tester.cpp				\
			)
HEADERS			= $(addprefix headers/,			\
				Header.cpp						\
			)
DELETE			= $(addprefix delete/,			\
				Delete.cpp						\
			)
GET				= $(addprefix get/,				\
				Get.cpp							\
			)
POST			= $(addprefix post/,			\
				Post.cpp						\
			)

#############################################
######			FILES .O			#########
#############################################

OBJ_DIR = objs
OBJ_DIR_TESTER = objs_tester

O_DIR = $(DIR_C:$(SRC)/%=$(OBJ_DIR)/%)
O_DIR_TESTER = $(DIR_TESTER:$(SRC_TESTER)/%=$(OBJ_DIR_TESTER)/%)

OBJS = $(FILES_C:$(SRC)/%.cpp=$(OBJ_DIR)/%.o)
OBJS_TESTER = $(FILES_TESTER:$(SRC_TESTER)/%.cpp=$(OBJ_DIR_TESTER)/%.o)

#############################################
######			FILES .O			#########
#############################################

#@printf "$(YELLOW)Compiling Object...$(RESET) %33.33s        \r" "$@"

all: $(NAME)

tester: $(NAME_TESTER)

full: $(NAME) $(NAME_TESTER)

$(NAME): $(OBJS) 
	@printf "$(GREEN)================================================$(RESET)"
	@printf "\n$(GREEN)================== WEB_SERVER ==================$(RESET)"
	@printf "\n$(GREEN)================================================$(RESET)"
	@printf "\n$(YELLOW)Compiling Object...$(RESET)"
	@printf "\n$(GREEN)-->[OK]                       $(RESET)"
	@printf "\n$(CYAN)Compiling $@..."
	-@$(CC) $(FLAGS) $(INC) $(OBJS) -o $(NAME)
	@echo "\n$(GREEN)-->[OK]$(RESET)\n"

$(NAME_TESTER): $(OBJS_TESTER) 
	@printf "$(GREEN)================================================$(RESET)"
	@printf "\n$(GREEN)==================== TESTEUR ===================$(RESET)"
	@printf "\n$(GREEN)================================================$(RESET)"
	@printf "\n$(YELLOW)Compiling Object...$(RESET)"
	@printf "\n$(GREEN)-->[OK]                       $(RESET)"
	@printf "\n$(CYAN)Compiling $@..."
	-@$(CC) $(FLAGS) $(INC_TESTER) $(OBJS_TESTER) -o $(NAME_TESTER)
	@echo "\n$(GREEN)-->[OK]$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRC)/%.cpp
	@mkdir -p objs/ $(O_DIR)
	@$(CC) $(FLAGS) -o $@ -c $< $(INC)

$(OBJ_DIR_TESTER)/%.o: $(SRC_TESTER)/%.cpp
	@mkdir -p objs/ $(O_DIR_TESTER)
	@$(CC) $(FLAGS) -o $@ -c $< $(INC_TESTER)

clean:
	@printf "$(GREEN)================================================$(RESET)\n"
	@printf "$(GREEN)==================== CLEAR =====================$(RESET)\n"
	@printf "$(GREEN)================================================$(RESET)\n"
	@printf "$(YELLOW)Clear Objects...$(RESET)                                                       "
	@printf "\n$(GREEN)-->[OK]$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR_TESTER)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAME_TESTER)
	@printf "$(YELLOW)Remove Executable...$(RESET)"
	@printf "\n$(GREEN)-->[OK] : $(NAME) removed$(RESET)\n\n"

re: fclean all

.SILENT: clean

.PHONY: all clean fclean re
