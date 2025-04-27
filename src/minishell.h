/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:59:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/24 15:23:38 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

# ifndef MAX_FD
#  define MAX_FD 1024
# endif

#include <readline/readline.h>
#include <readline/history.h>

extern pid_t	g_signal_pid;

typedef	struct s_token
{
	char	*value;			// the string without quotes
	int	type;			// command, argument, operator, etc...
	char	*expanded_value;	// after expansion
	char	*quote_mask;		// quoting context for each character: 'NNNDDS'
}	t_token;

typedef	struct s_env_node
{
	char	*key;
	char	*value;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env_list
{
	t_env_node	*head;		
	int	size;
}	t_env_list;

typedef struct s_redir_node
{
	int	type;	// HD or IN or TRUNC or APPEND
	char	*str;	// file name or LIM (heredoc)
	struct	s_redir_node	*next;
}	t_redir_node;

typedef	struct	s_ast_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	int	visited;
	int	type;
	char	**args;
	struct s_redir_node	*redirect;
}	t_ast_node;

typedef struct s_minishell
{
	t_env_list	*env_list;
	t_ast_node	*ast;
	int		pipe_count;
	int		*pipe_fds;
	int		last_exit;
	int		original_stdin;
	int		original_stdout;
	char		*line;
}	t_minishell;

enum token_types
{
	CMD,
	ARG,
	IN,
	HD,
	TRUNC,
	APPEND,
	FILENAME,
	PIPE,
	AND,
	OR,
	//OPEN_PAR,
	//CLOS_PAR
};

enum group_types
{
	COMMAND,
	REDIRECT,
};

// env.c
t_env_node	*set_node(char *key, char *value);
void	unset_node(t_env_node *node);
t_env_node	*init_node(char *line);
t_env_list	*init_env(void);
t_env_list	*populate_env(char **env);

// env_utils.c
char	*ft_getenv(char	*name, t_env_list *env);
int	ft_setenv(char *key, char *value, int overwrite, t_env_list *env);
void	ft_unsetenv(char *name, t_env_list *env);

// utils.c
int	is_equal(char *s1, char *s2);
char	*append_str(char *dest, char *src);
int	is_a_number(char *str);
char	**init_list(void);
char	**append_to_lst(char **l, char *new_s);

// builtins_utils.c
int	is_n_option(char *s);
int	change_directories(char *path);
void	export_no_args(t_env_list *env);
int	is_valid_env_name(char *str);
void	export_var(char *str, t_env_list *env);

// builtins.c;;
int	ft_echo(int ac, char **args);
int	ft_pwd(void);
int	ft_cd(int ac, char **args, t_env_list *env);
int	ft_export(int ac, char **args, t_env_list *env);
int	ft_unset(int ac, char **args, t_env_list *env);
int	ft_env(t_env_list *env);
void	ft_exit(int ac, char **args, t_minishell *sh);


// expansion.c
t_token	**expand_tokens(t_token **token_list, t_minishell *sh, t_env_list *env);

// expansion_utils.c
char	*init_str(void);
int	handle_tilde(char **res, t_token *tk, t_env_list *env);
char	**expanded_list(int ac, t_token **tk_list);

// tokenization.c
t_token	*init_token(char *str);
t_token	**populate_tokens(int ac, char **av);

// free_all.c
void	free_str_list(char **lst);
void	free_struct(t_minishell *sh);
void	free_token_list(t_token **tk_list);
void	free_ast(t_ast_node *node);

// exec.c
int	exec_external(char *n, char **args, t_minishell *sh);
int	exec_builtin(char **args, t_ast_node *node, t_minishell *sh);

// exec_utils.c
int	is_builtin(char *str);
int	ft_lstlen(char **args);
char	**convert_envp_to_array(t_env_list *envl);

// exec_path.c
char	*find_path(char *name, t_minishell *sh);

// exec_redirection.c
int	set_redirections(char **args, t_redir_node *redir, t_minishell *sh);

// tree.c
void	print_ast_node(t_ast_node *ast);
t_ast_node	*create_ast(t_token **tk_list);

// tree_traverse.c
int	cmd_node(t_ast_node *node, t_minishell *sh);
void	pipe_node(t_ast_node *node, t_minishell *sh);
void	dfs_ast(t_ast_node *node, t_minishell *sh);
void	function_dfs_ast(t_ast_node *node, t_minishell *sh, int (*f)(t_ast_node *, t_minishell *));

// tree_redirection.c
t_redir_node	*set_one_redir(t_redir_node *redir, t_token ***tkp);

// here_doc.c
int	here_doc(char *lim, t_minishell *sh);

// error.c
int	printf_fd(int fd, const char *s, ...);

// check_types.c
int	is_redirect(int type);
int	is_operator(int type);
int	is_command(int type);
int	is_pipe(int type);
int	is_file(int type);

// parsing.c
char	set_quote_character(char c, char new);
t_token	**init_token_list(char *line);
void	set_each_token_type(t_token ***tk_list_pt, int grp);
int	check_syntax(t_token **tk_list, t_minishell *sh);

// signals.c
void	set_signals(void);
#endif
