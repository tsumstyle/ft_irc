# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aroux <aroux@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/04 13:11:48 by aroux             #+#    #+#              #
#    Updated: 2025/10/06 14:19:06 by aroux            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors and emojis
GREEN = \033[0;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[0;31m
RESET = \033[0m
CHECK = ✅
CROSS = ❌
KEY = 🔧
TRASH = 🗑️

# Compiler setup
NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinc
SRC_DIR = src
OBJ_DIR = obj

# Sources and objects
SRC 	=	$(SRC_DIR)/main.cpp		\
			$(SRC_DIR)/Channel.cpp			$(SRC_DIR)/Client.cpp				$(SRC_DIR)/Server.cpp \
			$(SRC_DIR)/Server_signals.cpp	$(SRC_DIR)/Server_error_handler.cpp	$(SRC_DIR)/Server_utils.cpp \
			$(SRC_DIR)/utilities.cpp		$(SRC_DIR)/parser.cpp \
			$(SRC_DIR)/cmds/pass.cpp	$(SRC_DIR)/cmds/user.cpp	$(SRC_DIR)/cmds/nick.cpp \
			$(SRC_DIR)/cmds/join.cpp	$(SRC_DIR)/cmds/part.cpp	$(SRC_DIR)/cmds/quit.cpp \
			$(SRC_DIR)/cmds/invalid.cpp	$(SRC_DIR)/cmds/privmsg.cpp	$(SRC_DIR)/cmds/names.cpp \
			$(SRC_DIR)/cmds/ping.cpp	$(SRC_DIR)/cmds/list.cpp	$(SRC_DIR)/cmds/mode.cpp \
			$(SRC_DIR)/cmds/topic.cpp	$(SRC_DIR)/cmds/kick.cpp
			
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Default target
all: $(NAME)

# Linking
$(NAME): $(OBJ)
	@echo "$(BLUE)$(KEY) Linking...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@echo "$(GREEN)$(CHECK) Build complete: $(NAME)$(RESET)"
	@echo "   Usage: ./$(NAME) <port> <password>"

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)$(KEY) Compiling $< with $(CXXFLAGS)...$(RESET) "
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure obj/ exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean object files
clean:
	@echo "$(TRASH)$(RED) Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

# Clean all
fclean: clean
	@rm -f $(NAME)
	@echo "$(TRASH)$(RED) All clean.$(RESET)"

valgrind:
	valgrind --leak-check=full ./$(NAME) 8080 test

valfull:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	./$(NAME) 8081 test

valchild:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	--track-fds=yes --trace-children=yes --suppressions=readline_suppress.supp \
	./$(NAME) 8080 test

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re valgrind
