/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:59:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/17 19:17:06 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

// This is a linked hash table
// each node has a key and a value

typedef	struct s_token
{
	char	*value;			// the raw string
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

// utils.c
int	is_equal(char *s1, char *s2);

// builtins_utils.c
int	is_n_option(char *s);
void	change_directories(char *path);
char	*ft_getenv(char	*name, t_env_list *env);
int	ft_setenv(char *key, char *value, int overwrite, t_env_list *env);

// builtins.c
void	ft_echo(int ac, char **args);
void	ft_pwd(void);
void	ft_cd(int ac, char **args, t_env_list *env);

#endif
