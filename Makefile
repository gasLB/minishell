CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibft/includes -Iincludes
DFLAGS = $(CFLAGS) -g3

LIB_DIR = libft
LIB_NAME = $(LIB_DIR)/libft.a

SRC_DIR = src
SRC_FILES =	builtins2.c \
			builtins.c \
			builtins_utils.c \
			check_types.c \
			env.c \
			env_utils.c \
			error.c \
			exec.c \
			exec_cmd_utils.c \
			exec_path.c \
			exec_redirection.c \
			expansion.c \
			expansion_utils.c \
			free_all.c \
			free_ast.c \
			here_doc.c \
			main.c \
			minishell.c \
			parsing.c \
			parsing_utils.c \
			pipes.c \
			signals.c \
			syntax.c \
			tokenization.c \
			tree_new.c \
			tree_redirection.c \
			tree_traverse.c \
			tree_traverse_utils.c \
			tree_utils.c \
			utils.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

NAME = minishell

all: $(LIB_NAME) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME):
	$(MAKE) -C $(LIB_DIR)

$(NAME): $(OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

debug: fclean
	$(MAKE) -C $(LIB_DIR) debug
	$(MAKE) CFLAGS="$(DFLAGS)" all

clean:
	$(MAKE) -C $(LIB_DIR) clean
	rm -rf $(OBJ_DIR)
	rm -rf $(OBJ_BNS_DIR)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	rm -f $(NAME)
	rm -f $(NAME_BNS)

re: fclean all

.PHONY: all debug bonus debug_bonus clean fclean re
