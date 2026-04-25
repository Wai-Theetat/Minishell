# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/25 15:17:31 by tdharmar          #+#    #+#              #
#    Updated: 2026/04/25 16:07:04 by tdharmar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME = minishell

# Directory
SRC_DIR 	= srcs
INC_DIR 	= includes
OBJ_DIR 	= obj

LIBFT_DIR	= libft

LIBFT = $(LIBFT_DIR)/libft.a

# Compiler & Flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
INCLUDES	= -I $(INC_DIR) -I $(LIBFT_DIR)
LIB			= -L $(LIBFT_DIR) -lft -lreadline -lhistory

# Define Hostname  
HOST		= $(shell hostname -s)
INCLUDES	+= -D HOSTNAME=\"$(HOST)\"

# Sources
SRC_FILES = main.c \

#NIXOS
ifeq ($(HOST), nixos)
	INCLUDES += $(shell pkg-config --cflags readline)
	LIB      += $(shell pkg-config --libs readline)
endif

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# COLORS
DEFAULT	= \033[0;39m
RED		= \033[1;91m
GREEN	= \033[1;92m

# RULES
all : $(NAME)

$(LIBFT):
	$(MAKE) --silent -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(NAME)
	@echo -e "$(GREEN)$(NAME) Compiled SUCCESS!!$(DEFAULT)"

#compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/minishell.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make --silent -C $(LIBFT_DIR) clean
	@echo -e "$(RED)Object files cleaned$(DEFAULT)"

fclean: clean
	rm -f $(NAME)
	@make --silent -C $(LIBFT_DIR) fclean
	@echo -e "$(RED)ALL CLEANED!!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re 