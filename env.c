/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:13:13 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/12 19:05:32 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_node	*init_node(char **key, char **value)
{
	t_env_node	*env_node;

	env_node = malloc(sizeof(t_env_node));
	if (!env_node)
		return (NULL);
	env_node->key = key;
	env_node->value = value;
	env_node->next = NULL;
	return (env_node);
}

t_env_list	*init_env(void)
{
	t_env_list	*env_list;

	env_list = malloc(sizeof(t_env_list));
	if (!env_list)
		return (NULL);
	env_list->head = NULL;
	env_list->size = 0;
	return (env_list);
}

char	*init_key(char *env_line)
{

}

char	*init_key(char *env_line)
{

}

t_env_list	*populate_env(char **env)
{
	t_env_list	*env_list;
	t_env_node	*current_node;
	int	i;


	i = 0;
	env_list = init_env();
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
		{
			node = init_node(init_key(env[i]), init_value(env[i]));
			if (!env_list->head)
				env_list->head = 
			
		}
	}
	
}


// I also need a ft_gentenv() and a ft_setenv()
