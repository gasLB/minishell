/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:38:52 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/22 20:12:23 by gfontagn         ###   ########.fr       */
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


void	ft_export(int ac, char **args, t_env_list *env)
{
	int	i;

	i = 1;
	if (ac <= 1)
		(export_no_args(env), exit(1));
	while (i < ac)
	{
		if (is_valid_env_name(args[i]))
			export_var(args[i], env);
		else
			ft_printf("minishell: export: %s: not a valid identifier\n", args[i]);
		i++;
	}
}

void	ft_unset(int ac, char **args, t_env_list *env)
{

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

/*

EXPORT
- When you run export on its own, you should display env variables sorted in this shape: 
```
$ declare -x HOME="/USER/fraqioui"
$ declare -x var1
$ declare -x var=""
```
- Export with a variable name should add this var to the environment variables which is the env linked list.
- The variable Should be an identifier which means that the var should start with an alphabet (uppercase/lowercase)
	or underscore character. In addition the var could contain a number.
	Examples of valid identifiers: Var12, _var12, v_1ar, var1_, _ , ...etc
	Examples of invalid identifiers: 1Var, @var, v+ar, ...etc
- Before exporting the var you should check first if it already exists.
Ex:
```
$ export var
//now, var already exists in env variables
$ export var
//in this case you should not update this variable
$ export var=hello
//now, you should update the value of this variable.
```
- If there is a plus before an equal symbol you should append the var value, if there is just an equal symbol you should overwrite the var value.
- Examples of export:
```
$ export var
$ export =value
$ export var=		equivalent de var=""
$ export var=""
$ export var====value
$ export var+=value
$ export var1 var2 var3 var4
```
if one name is not valit, all the previous ones are exported nontheless
*/

// if just variable name
// 	if valid:
// 		if exist:
// 			do nothing
// 		else:
// 			create var
// 	else:
// 		minishell: export: {arg}: not a valid identifier
// else:
// 	if valid:
// 		if exists:
// 			if +=:
// 				append {value} at the end
// 			if =:
// 				overwrite {value} of {key}
// 		else:
// 			create var {key} = {value}
// 	else:
// 		minishell: export: {arg}: not a valid identifier
//
//

