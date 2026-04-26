NAME        = minishell
SRC_DIR     = srcs
OBJ_DIR     = obj
INC_DIR     = includes
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

CC          = cc
HOST        = $(shell hostname -s)

SRCS        = $(shell find $(SRC_DIR) -name '*.c')
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS        = $(OBJS:.o=.d)
INC_DIRS    = $(shell find $(SRC_DIR) -type d)

CFLAGS      = -Wall -Wextra -Werror -g3
CFLAGS      += -MMD -MP
CFLAGS      += $(addprefix -I, $(INC_DIRS))
CFLAGS      += -I $(INC_DIR) -I $(LIBFT_DIR)
CFLAGS      += -D HOSTNAME=\"$(HOST)\"

LIB         = -L $(LIBFT_DIR) -lft -lreadline

ifeq ($(HOST), nixos)
    CFLAGS  += $(shell pkg-config --cflags readline)
    LIB     += $(shell pkg-config --libs readline)
endif

# Colors
GREEN       = \033[1;92m
RED         = \033[1;91m
RESET       = \033[0;39m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make --silent -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(NAME)
	@echo -e "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Debug with address sanitizer
debug: CFLAGS += -fsanitize=address
debug: re

# Norminette
norm:
	@norminette $(shell find $(SRC_DIR) -name '*.[ch]')
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

.PHONY: all clean fclean re debug norm