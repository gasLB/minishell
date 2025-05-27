/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:28:18 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/26 21:25:43 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <termios.h>
#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"

int	ft_unset(int ac, char **args, t_env_list *env)
{
	int	i;

	i = 0;
	while (i < ac)
	{
		if (is_valid_env_name(args[i]))
			ft_unsetenv(args[i], env);
		else
			printf_fd(2, "minishell: unset: %s: " INVAL, args[i]);
		i++;
	}
	return (0);
}

int	ft_env(t_env_list *env)
{	
	t_env_node	*node;

	node = env->head;
	while (node)
	{
		if (node->value)
			ft_printf("%s=%s\n", node->key, node->value);
		node = node->next;
	}
	return (0);
}

void	exit_with_number(int n, char **args, t_minishell *sh)
{
	if (sh->last_command_type != -1 || sh->pipe_count)
		sh->last_exit = n;
	else
	{
		free_str_list(args);
		close_all_pipes(sh);
		free_struct(sh);
		exit(n);
	}
}

void	ft_exit(int ac, char **args, t_minishell *sh)
{
	ft_printf("exit\n");
	if (ac == 0)
		exit_with_number(2, args, sh);
	else if (!is_a_number(args[1]) || !is_correct_size_exit(args[1]))
	{
		printf_fd(2, "minishell: exit: %s: " NUMERIC_ARG, args[1]);
		exit_with_number(2, args, sh);
	}
	else if (ac > 1)
	{
		printf_fd(2, "minishell: exit: " TOO_MANY);
		sh->last_exit = 1;
	}
	else
		exit_with_number((int)(ft_atoll(args[1]) % 256), args, sh);
}
