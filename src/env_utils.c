/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:41:29 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 19:45:44 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

void	ft_unsetenv(char *name, t_env_list *env)
{
	t_env_node	*node;
	t_env_node	*to_remove;

	node = env->head;
	while (node->next)
	{
		if (is_equal(name, (node->next)->key))
		{
			to_remove = node->next;
			node->next = to_remove->next;
			unset_node(to_remove);
			env->size -= 1;
			break ;
		}
		node = node->next;
	}
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

void	update_existing_node(t_env_node *node, char *value, int overwrite)
{
	if (overwrite && value)
	{
		free(node->value);
		node->value = ft_strdup(value);
	}
	else if (value)
		node->value = append_str(node->value, value);
}

int	ft_setenv(char *key, char *value, int overwrite, t_env_list *env)
{
	t_env_node	*node;
	t_env_node	*new_node;

	node = env->head;
	if (!node)
		env->head = set_node(key, value);
	while (node)
	{
		if (is_equal(key, node->key))
		{
			update_existing_node(node, value, overwrite);
			return (0);
		}
		if (!node->next)
		{
			new_node = set_node(key, value);
			if (!new_node)
				return (1);
			node->next = new_node;
			env->size++;
			return (0);
		}
		node = node->next;
	}
	return (0);
}
