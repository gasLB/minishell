/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:59:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/12 19:05:06 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

// This is a linked hash table
// each node has a key and a value

typedef	struct s_env_node
{
	char	*key;
	char	*value;
	t_env_node	*next;
}	t_env_node;

typedef struct s_env_list
{
	t_env_node	*head;		
	int	size;
}	t_env_list;

// builtins_utils.c
int	is_n_option(char *s);
void	change_directories(char *path);

// builtins.c
void	ft_echo(int ac, char **args);
void	ft_pwd(void);
void	ft_cd(int ac, char **args);

#endif
