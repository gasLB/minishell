/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:59:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/04 21:11:05 by gfontagn         ###   ########.fr       */
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

# define RETRIEVE "error retrieving current directory: "
# define NO_ACCESS "cannot access parent directories: "

# define NO_FILE "No such file or directory\n"
# define TOO_MANY "too many arguments\n"
# define ARG_TOO_LONG "Argument list too long\n"
# define NOT_FOUND "command not found\n"
# define NUMERIC_ARG "numeric argument required\n"
# define PERMISSION "Permission denied\n"
# define MEMORY "Cannot allocate memory\n"
# define NO_HOME "HOME not set\n"
# define NO_OLDPWD "OLDPWD not set\n"
# define INVAL "not a valid identifier\n"
# define EXP_VALUE "export %s=\"%s\"\n"
# define UNEXPECTED_NL "syntax error near unexpected token `newline'\n"
# define UNEXPECTED_S "syntax error near unexpected token `%s'\n"
# define NO_FDS "Too many open files\n"

# include <readline/readline.h>
# include <readline/history.h>

extern pid_t	g_signal_pid;

typedef struct s_token
{
	char	*value;
	int		type;
	char	*expanded_value;
	char	*quote_mask;
	char	*transition_mask;
}	t_token;

typedef struct s_env_node
{
	char				*key;
	char				*value;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env_list
{
	t_env_node	*head;		
	int			size;
}	t_env_list;

typedef struct s_redir_node
{
	char				*str;
	int					type;
	struct s_redir_node	*next;
}	t_redir_node;

typedef struct s_ast_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	int					visited;
	int					type;
	char				**args;
	struct s_redir_node	*redirect;
}	t_ast_node;

typedef struct s_minishell
{
	t_env_list	*env_list;
	t_ast_node	*ast;
	int			pipe_count;
	int			*pipe_fds;
	int			last_exit;
	int			original_stdin;
	int			original_stdout;
	char		*line;
}	t_minishell;

enum e_token_types
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
};

enum e_group_types
{
	COMMAND,
	REDIRECT,
};

// env.c
t_env_node	*set_node(char *key, char *value);
void		unset_node(t_env_node *node);
t_env_node	*init_node(char *line);
t_env_list	*init_env(void);
t_env_list	*populate_env(char **env, int i);

// env_utils.c
char		*ft_getenv(char	*name, t_env_list *env);
int			ft_setenv(char *key, char *valu, int overwrite, t_env_list *env);
void		ft_unsetenv(char *name, t_env_list *env);

// utils.c
int			is_equal(char *s1, char *s2);
char		*append_str(char *dest, char *src);
int			is_a_number(char *str);
char		**init_list(void);
char		**append_to_lst(char **l, char *new_s);

// builtins_utils.c
int			is_n_option(char *s);
int			change_directories(char *path);
void		export_no_args(t_env_list *env);
int			is_valid_env_name(char *str);
void		export_var(char *str, t_env_list *env);

// builtins.c;;
int			ft_echo(int ac, char **args);
int			ft_pwd(void);
int			ft_cd(int ac, char **args, t_env_list *env);
int			ft_export(int ac, char **args, t_env_list *env);
int			ft_unset(int ac, char **args, t_env_list *env);
int			ft_env(t_env_list *env);
void		ft_exit(int ac, char **args, t_minishell *sh);

// expansion.c
t_token		**expand_tokens(t_token **token_list, t_minishell *sh, \
		t_env_list *env);

// expansion_utils.c
int			translation(char **res, t_token *tk, int i);
char		*init_str(void);
int			handle_tilde(char **res, t_token *tk, t_env_list *env);
char		**expanded_list(int ac, t_token **tk_list);

// tokenization.c
t_token		*init_token(char *str);

// free_all.c and free-ast.c
void		free_str_list(char **lst);
void		free_struct(t_minishell *sh);
void		free_token_list(t_token **tk_list);
void		free_ast(t_ast_node *node);

// exec.c
int			exec_external(char *n, char **args, t_minishell *sh);
int			exec_builtin(char **args, t_minishell *sh);

// exec_utils.c
int			is_builtin(char *str);
int			ft_lstlen(char **args);
char		**convert_envp_to_array(t_env_list *envl);

// exec_path.c
char		*find_path(char *name, t_minishell *sh);

// exec_redirection.c
int			set_redirections(char **args, t_redir_node *redir, \
		t_minishell *sh);

// tree.c
void		print_ast_node(t_ast_node *ast);
t_ast_node	*create_ast(t_token **tk_list);

// tree_utils.c
t_ast_node	*init_ast_node(void);
t_ast_node	*set_ast_node(int type, char **args, t_redir_node *red);
int			get_precedence(t_token *token);

// tree_redirection.c
void		set_one_redir(t_redir_node **redir, t_token ***tkp);

// tree_traverse.c
int			cmd_node(t_ast_node *node, t_minishell *sh);
int			pipe_node(t_ast_node *node, t_minishell *sh);
void		dfs_ast(t_ast_node *node, t_minishell *sh);
void		function_dfs_ast(t_ast_node *node, t_minishell *sh, \
		int (*f)(t_ast_node *, t_minishell *));

// tree_traverse_utils.c
int			null_cmd_node(t_ast_node *node, t_minishell *sh);
int			is_directory(char **args);

// here_doc.c
int			here_doc(char *lim, t_minishell *sh);

// error.c
int			printf_fd(int fd, const char *s, ...);

// check_types.c
int			is_redirect(int type);
int			is_operator(int type);
int			is_command(int type);
int			is_pipe(int type);
int			is_file(int type);

// parsing.c
char		get_quote_character(char c, char new, int i, int *last);
t_token		**init_token_list(char *line);
void		set_each_token_type(t_token ***tk_list_pt, int grp);
int			check_syntax(t_token **tk_list, t_minishell *sh);
int			end_op(char *line, int end, char quote);

// parsing_utils
char		*correct_substr(char *line, int start, int end);
int			update_token_end(char *line, char quote, int end);
int			token_basic_type(t_token *token);

// signals.c
void		set_signals(void);

// minishell.c
void		minishell(t_minishell *sh, t_env_list *env_list);

// pipes.c
int	dup_pipe(t_ast_node *n, int fd[2], int or_std[2], t_minishell *sh);
void	add_pipe_fd(int fd1, int fd2, t_minishell *sh);

#endif
