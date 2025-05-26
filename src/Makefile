
NAME = minishell
FLAGS = -Wall -Werror -Wextra -g
LIBFTDIR = ../libftprintf
LIBFT = -L$(LIBFTDIR)/libft -lft
PRINTF = -L$(LIBFTDIR) -lftprintf 
READLINE = -I. -lreadline

CC = cc

SRC := builtins.c builtins2.c builtins_utils.c check_types.c check_types2.c \
	   env.c env_utils.c error.c exec.c exec_cmd_utils.c exec_path.c \
       exec_redirection.c expansion.c expansion_utils.c free_all.c \
       free_ast.c here_doc.c main.c minishell.c parsing.c parsing_utils.c \
       signals.c syntax.c tokenization.c tree_new.c tree_redirection.c \
       tree_traverse.c tree_traverse_utils.c tree_utils.c utils.c pipes.c

OBJ := $(SRC:.c=.o)

all: $(LIBFTDIR)/libftprintf.a $(NAME)

$(LIBFTDIR)/libftprintf.a:
	make -C $(LIBFTDIR)

$(NAME): $(OBJ)	
	$(CC) -g $(OBJ) $(FLAGS) $(LIBFT) $(READLINE) $(PRINTF) -o $(NAME)

%.o: %.c 
	$(CC) -g $(FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
