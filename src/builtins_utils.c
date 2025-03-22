/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:50:00 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/22 19:42:29 by gfontagn         ###   ########.fr       */
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

int	is_valid_env_name(char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
		{
			if (!(str[i] == '+' && str[i + 1] == '='))
				return (0);
		}
		i++;
	}
	return (1);
}

void	export_var(char *str, t_env_list *env)
{
	char	*pos;
	char	*key;
	char	*value;
	int	overwrite;

	overwrite = 1;
	pos = ft_strchr(str, '=');
	if (!pos)
		ft_setenv(str, NULL, 0, env);
     	else
	{
		if (*(pos - 1) == '+')
			overwrite = 0;
		key = ft_substr(str, 0, pos - str + overwrite - 1); 
		// if ovewrite -> len - 1
		// if not -> len
		value = ft_substr(pos + 1, 0, ft_strlen(pos - 1));
		if (!key || !value)
			return;
		ft_setenv(key, value, overwrite, env);
	}
}

void	export_no_args(t_env_list *env)
{
	t_env_node	*node;

	node = env->head;
	while (node)
	{
		if (node->value == NULL)
			ft_printf("export %s\n", node->key);
		else
			ft_printf("export %s=\"%s\"\n", node->key, node->value);
		node = node->next;
	}
}	
