/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:50:00 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/17 18:36:46 by wbeschon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	change_directories(char *path, t_env_list *env)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf_fd(2, "chdir: " RETRIEVE "getcwd: " NO_ACCESS NO_FILE);
		return (1);
	}
	free(cwd);
	if (chdir(path) != 0)
	{
		printf_fd(2, "minishell: cd: %s: " NO_FILE, path);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	ft_setenv("PWD", pwd, 1, env);
	free(pwd);
	return (0);
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

void	export_var(char *str, t_env_list *env, int overwrite)
{
	char	*pos;
	char	*key;
	char	*value;
	char	*trimed;

	pos = ft_strchr(str, '=');
	if (!pos)
		ft_setenv(str, NULL, overwrite, env);
	else
	{
		if (*(pos - 1) == '+')
			overwrite = 0;
		key = ft_substr(str, 0, pos - str + overwrite - 1);
		value = init_str();
		if (!(*(pos + 1)))
			ft_setenv(key, value, overwrite, env);
		else
		{
			free(value);
			value = ft_substr(pos + 1, 0, ft_strlen(pos - 1));
			trimed = ft_strtrim(value, " ");
			(ft_setenv(key, trimed, overwrite, env), free(trimed));
		}
		(free(key), free(value));
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
			ft_printf(EXP_VALUE, node->key, node->value);
		node = node->next;
	}
}
