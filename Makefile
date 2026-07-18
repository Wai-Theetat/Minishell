NAME		= minishell
CC			= cc
# CFLAGS		= -Wall -Wextra -Werror -g3
CFLAGS		=  -g3
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

UNAME		= $(shell uname -s)
ifeq ($(UNAME), Darwin)
	READLINE	= $(shell brew --prefix readline)
	CFLAGS	+= -I $(READLINE)/include
	LIB	+= -L $(READLINE)/lib
endif

SRC_DIR		= srcs
OBJ_DIR		= obj

SRCS			=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/signal/signal.c \
				$(SRC_DIR)/signal/signal_exec.c \
				$(SRC_DIR)/lexer/token.c \
				$(SRC_DIR)/lexer/lexer.c \
				$(SRC_DIR)/lexer/lexer_word.c  \
				$(SRC_DIR)/utilities/builtin_utils.c \
				$(SRC_DIR)/utilities/checker.c \
				$(SRC_DIR)/utilities/printer.c \
				$(SRC_DIR)/utilities/write_err_message.c \
				$(SRC_DIR)/utilities/extra_helper.c \
				$(SRC_DIR)/utilities/input.c \
				$(SRC_DIR)/executor/find_exec.c \
				$(SRC_DIR)/executor/exec_simple.c \
				$(SRC_DIR)/executor/exec_second.c \
				$(SRC_DIR)/executor/exec_utils.c \
				$(SRC_DIR)/executor/exec_n.c \
				$(SRC_DIR)/parser/parser.c \
				$(SRC_DIR)/parser/syntax.c \
				$(SRC_DIR)/parser/heredoc.c \
				$(SRC_DIR)/parser/heredoc_child.c \
				$(SRC_DIR)/expand/expand.c \
				$(SRC_DIR)/builtins/builtin_echo.c \
				$(SRC_DIR)/builtins/builtin_env.c \
				$(SRC_DIR)/builtins/builtin_exit.c \
				$(SRC_DIR)/builtins/builtin_pwd.c \
				$(SRC_DIR)/builtins/buitlin_cd.c \
				$(SRC_DIR)/builtins/builtin_export.c \
				$(SRC_DIR)/builtins/builtin_unset.c \
				$(SRC_DIR)/env/env_init.c \
				$(SRC_DIR)/env/env_utils.c \

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

GREEN		= \033[1;92m
RED			= \033[1;91m
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
