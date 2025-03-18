/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:38:52 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/18 18:53:46 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <termios.h>
#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"

void	ft_echo(int ac, char **args)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	while (i < ac && is_n_option(args[i]))
		(i++, n_option = 1);
	while (i < ac)
	{
		ft_printf("%s", args[i]);
		if (i + 1 == ac && n_option == 0)
			ft_printf("\n");
		else if (i + 1 != ac)
			ft_printf(" ");
		i++;
	}
	//free_args(args);
	exit(0);	
}

void	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_printf("%s\n", cwd);
	if (cwd)
		free(cwd);
	exit(0);
}

void	ft_cd(int ac, char **args, t_env_list *env)
{
	char	*path;

	if (ac > 2)
		(ft_printf("minishell: cd: too many arguments\n"), exit(1)); //need proper exit function
	else if (ac == 1)
	{
		path = ft_getenv("HOME", env);
		if (!path)
			(ft_printf("minishell: cd: HOME not set\n"), exit(1));
	}
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '-')
	{
		path = ft_getenv("OLDPWD", env);
		if (!path)
			(ft_printf("minishell: cd: OLDPWD not set\n"), exit(1));
	}
	else
		path = args[1];
	change_directories(path);
	exit(1);
}

void	ft_export(int ac, char **args)
{
	(void)ac;
	(void)args;
	return;
	// minishell: export: '...': not a valid identifier
	// -> wrong syntax of the variable name: non-authorized character
	// -> no '=' or '=' at the end or at the wrong place (end or beginning)
	// if one name is not valit, all the previous ones are exported nontheless
	//
	// if no arguments: list all the exported variables
	// -> understand the difference between 'exported' variables and non-exported
	// it this necessary?
}
/*
list of builtins to implement: 
echo (-n)
cd
pwd
export
unset
env
exit

Each builtin should exit with the correct status

Specific error message if cwd is removed for pwd and cd:
pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory

-> to check later when I have the input system and a valid parsing

all args need to be freed before exit : printf_err() -> writes on stdr AND can format


*/
