/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:38:52 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/12 18:04:31 by gfontagn         ###   ########.fr       */
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

// list of builtins to implement: 
// echo (-n)
// cd
// pwd
// export
// unset
// env
// exit
//
// Each builtin should exit with the correct status
//
// If I consider the parsing complete, I must reuse some of the pipex logic to pipe
// those builtins alongside any command in any order
// temporary compile command:
// ccw builtins.c -L./libftprintf/libft -lft -L./libftprintf -lftprintf

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
	exit(0);	
}

// specific error message if cwd is removed for pwd and cd:
//
// pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
// chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
//
// -> to check later when I have the input system and a simplified parsing

void	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_printf("%s\n", cwd);
	if (cwd)
		free(cwd);
	exit(0);
}

void	ft_cd(int ac, char **args)
{
	char	*path;

	if (ac > 2)
		(ft_printf("minishell: cd: too many arguments\n"), exit(1));
	else if (ac == 1)
	{
		path = getenv("HOME");
		if (!path)
			(ft_printf("minishell: cd: HOME not set\n"), exit(1));
	}
	else if (ft_strlen(args[1]) == 1 && args[1][0] == '-')
	{
		path = getenv("OLDPWD");
		if (!path)
			(ft_printf("minishell: cd: OLDPWD not set\n"), exit(1));
	}
	else
		path = args[1];
	change_directories(path);
	exit(1);
}

// ENVIRONMENT:
//
// Seems that I was wrong about environment
// other guys have basically implemented their own environment structure : linked list or realloc
// So maybe using getenv() isn't a good idea, should have our own
// Maybe then use getenv() to initialize it with global environment
// 
// non-exported variables: only visible to the shell itself and not child processes

void	ft_export(int ac, char **args)
{
}

int	main(int ac, char **av, char **env)
{
	int	pid;

	(void)env;
}

// all args need to be freed before exit -> specific function
//
// mixed error handling perror or printf ? How to format with perror ?
// what about wrong number of arguments ? Should they be handled inside each function or 
// before executing each function ?
//
// printf_err() -> writes on stdr AND can format
//
