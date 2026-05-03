NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
CFLAGS		+= -MMD -MP

INC_DIR		= includes
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

HOST		= $(shell hostname -s)
CFLAGS		+= -I $(INC_DIR) -I $(LIBFT_DIR)
CFLAGS		+= -I srcs/shell -I srcs/lexer -I srcs/parser
CFLAGS		+= -I srcs/expand -I srcs/executor -I srcs/signals -I srcs/utils
CFLAGS		+= -D HOSTNAME=\"$(HOST)\"

LIB			= -L $(LIBFT_DIR) -lft -lreadline

ifeq ($(HOST), nixos)
	CFLAGS	+= $(shell pkg-config --cflags readline)
	LIB	+= $(shell pkg-config --libs readline)
endif

SRC_DIR		= srcs
OBJ_DIR		= obj

SRCS		=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/lexer/token.c \
				$(SRC_DIR)/lexer/lexer.c \


OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

GREEN		= \033[1;92m
RED		= \033[1;91m
RESET		= \033[0;39m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make --silent -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(NAME)
	@echo -e "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

norm:
	@norminette $(INC_DIR) $(SRC_DIR)
	@make --silent -C $(LIBFT_DIR) norm

clean:
	@rm -rf $(OBJ_DIR)
	@make --silent -C $(LIBFT_DIR) clean
	@echo -e "$(RED)Objects cleaned$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make --silent -C $(LIBFT_DIR) fclean
	@echo -e "$(RED)All cleaned$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re norm