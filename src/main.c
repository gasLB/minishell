/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 19:28:12 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

pid_t	g_signal_pid;

void	minishell_start(void)
{
	ft_printf("___  ____       _ _____ _          _ _\n");
	ft_printf("|  \\/  (_)     (_)  ___| |        | | |\n");
	ft_printf("| .  . |_ _ __  _\\ `--.| |__   ___| | |\n");
	ft_printf("| |\\/| | | '_ \\| |`--. \\ '_ \\ / _ \\ | |\n");
	ft_printf("| |  | | | | | | /\\__/ / | | |  __/ | |\n");
	ft_printf("\\_|  |_/_|_| |_|_\\____/|_| |_|\\___|_|_|\n\n");
	set_signals();
}

t_minishell	*init_shell(t_env_list *env_list)
{
	t_minishell	*sh;

	sh = malloc(sizeof(t_minishell));
	if (!sh)
		return (NULL);
	g_signal_pid = 0;
	sh->env_list = env_list;
	sh->last_exit = 0;
	sh->pipe_count = 0;
	sh->pipe_fds = NULL;
	sh->line = NULL;
	sh->ast = NULL;
	sh->original_stdin = dup(STDIN_FILENO);
	sh->original_stdout = dup(STDOUT_FILENO);
	return (sh);
}

void	update_shlvl(t_env_list *envl)
{
	char	*shlvl_char;
	int		shlvl;

	shlvl_char = ft_getenv("SHLVL", envl);
	if (shlvl_char)
	{
		shlvl = ft_atoi(shlvl_char) + 1;
		shlvl_char = ft_itoa(shlvl);
		ft_setenv("SHLVL", shlvl_char, 1, envl);
		free(shlvl_char);
	}
	else
		ft_setenv("SHLVL", "1", 1, envl);
}

void	init_env_variables(t_env_list *env)
{
	char	*cwd;

	update_shlvl(env);
	cwd = getcwd(NULL, 0);
	ft_setenv("PWD", cwd, 1, env);
	ft_setenv("OLDPWD", NULL, 1, env);
	free(cwd);
}

int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_minishell	*sh;

	(void)ac;
	(void)av;
	minishell_start();
	env_list = populate_env(env, -1);
	sh = init_shell(env_list);
	init_env_variables(env_list);
	minishell(sh, env_list);
	return (0);
}

/*
first I get tk_list from init_token_list
then need to set each token type with set_each_token_type()
then need to check syntax with check_syntax() and continue; if error in syntax
then expand values
then create the AST with create_ast
then free tk_list
then traverse the AST and execute with dfs_ast
Raw input → Tokenization → Expansion → Execution

ERRORS:

- [X] couleurs minishell
- [X] leaks with cd
- [X] leaks external builtins with unset 
- [X] no syntax check with unset
- [X] command names with full path
- [X] error with export HOME= with preexisting env variables
- [X] UPDATE SHLVL
- [X] init correct shell if launched with env -i
- [X] segfault if unset HOME then cd tilde
- [X] cat | ls or exit | ls should display ls before C^C
	-> wrong parsing and tree creation: I mix pipes and operators
- [X] pb redirection -> don't reset the original stds
- [X] mixed arguments and redirections: echo hola > bonjour salut
	-> I think to correct during tokenization
- [X] chain of here_doc redirections
	-> we first need to take limiters in the order of apparition
- [X] signals inside here_doc
*/
