/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 18:13:00 by gfontagn         ###   ########.fr       */
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

int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_minishell	*sh;

	(void)ac;
	(void)av;
	minishell_start();
	env_list = populate_env(env, -1);
	sh = init_shell(env_list);
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

- [ ] leaks with args and not found command -> rebuild this part
*/
