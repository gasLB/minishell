/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:59:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/22 19:42:52 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_minishell
{
	t_env_list	*env_list;
	int		last_exit;
}	t_minishell;

// env.c
t_env_node	*set_node(char *key, char *value);
t_env_node	*init_node(char *line);
t_env_list	*init_env(void);
t_env_list	*populate_env(char **env);

// env_utils.c
char	*ft_getenv(char	*name, t_env_list *env);
int	ft_setenv(char *key, char *value, int overwrite, t_env_list *env);

// utils.c
int	is_equal(char *s1, char *s2);
char	*ft_realloc_str(char *str, int new_size);
char	*append_str(char *dest, char *src);

// builtins_utils.c
int	is_n_option(char *s);
void	change_directories(char *path);
void	export_no_args(t_env_list *env);
int	is_valid_env_name(char *str);
void	export_var(char *str, t_env_list *env);

// builtins.c
void	ft_echo(int ac, char **args);
void	ft_pwd(void);
void	ft_cd(int ac, char **args, t_env_list *env);
void	ft_export(int ac, char **args, t_env_list *env);

// expansion.c
t_token	**expand_tokens(t_token **token_list, t_minishell *sh, t_env_list *env);

// expansion_utils.c
char	*init_str(void);
int	handle_tilde(char **res, t_token *tk, t_env_list *env);

// tokenization.c
t_token	**populate_tokens(int ac, char **av);

#endif
