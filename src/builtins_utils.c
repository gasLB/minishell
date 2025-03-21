/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:50:00 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/17 18:34:26 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	is_n_option(char *s)
{
	int	i;
	int	size;

	size = ft_strlen(s);
	if (ft_strncmp(s, "-n", 2) != 0)
		return (0);
	i = 2;
	while (i < size)
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	change_directories(char *path)
{
	if (chdir(path) != 0)
	{
		ft_printf("minishell: cd: %s: No such file or directory\n", path);
		exit(1);
	}
	exit(0);
}

char	*ft_getenv(char	*name, t_env_list *env)
{
	t_env_node	*node;

	node = env->head;
	while (node)
	{
		if (is_equal(name, node->key))
			return (node->value);
		node = node->next;
	}
	return (NULL);
}

int	ft_setenv(char *key, char *value, int overwrite, t_env_list *env)
{
	t_env_node	*node;
	t_env_node	*new_node;
	
	node = env->head;
	while (node->next)
	{
		if (is_equal(key, node->key))
		{
			if (overwrite)
			{
				free(node->value);
				node->value = ft_strdup(value); // should I duplicate ?
			}
			return (0);
		}
		node = node->next;
	}
	new_node = set_node(key, value);
	if (!new_node)
		return (1);
	node->next = new_node;
	env->size++;
	return (0);
}
