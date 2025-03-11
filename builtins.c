/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:38:52 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/11 18:24:41 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
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
// temporary compile command; ccw builtins.c -L./libftprintf/libft -lft -L./libftprintf -lftprintf

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
	if (ac > 2)
		(perror("minishell: cd: too many arguments"), exit(1));
	if (ac == 1)
		chdir("");
	else if (ac == 2)
	{
		if (chdir(args[1]) != 0)
		{
			ft_printf("minishell: cd: %s:\
					No such file or directory", args[1]); // STDERR !
			exit(1);
		}
	}
	exit(0);
	// seems to work but only in the child process ?
}

int	main(int ac, char **av, char **env)
{
	(void)env;
	ft_cd(ac, av);
}

// all args need to be freed before exit -> specific function
//
// mixed error handling perror or printf ? How to format with perror ?
// what about wrong number of arguments ? Should they be handled inside each function or 
// before executing each function ?
//
// printf_err() -> writes on stdr AND can format
//
