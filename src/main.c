/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:38:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/24 15:21:48 by gfontagn         ###   ########.fr       */
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

void	print_tokens(t_token **tk_list)
{
	int	i;

	i = 0;
	while (tk_list[i])
	{
		ft_printf("--- %d ---\n", i);
		ft_printf("value : %s\n", tk_list[i]->value);
		ft_printf("type : %d\n", tk_list[i]->type);
		ft_printf("quote mask : %s\n", tk_list[i]->quote_mask);
		ft_printf("exp value : %s\n", tk_list[i]->expanded_value);
		ft_printf("---------\n");
		i++;
	}
}

void	set_standard_fds(t_minishell *sh)
{
	dup2(sh->original_stdin, STDIN_FILENO);
	dup2(sh->original_stdout, STDOUT_FILENO);
}

int	only_space(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	minishell(t_minishell *sh, t_env_list *env_list)
{
	t_token		**token_list;

	while (1)
	{
		set_standard_fds(sh);
		sh->line = readline("\e[35m\e[1mMinishell> \e[0m");
		if (sh->line == NULL)
			(ft_printf("\n"), exit(0));
		if (only_space(sh->line))
		{
			free(sh->line);
			continue;
		}	
		add_history(sh->line);
		token_list = init_token_list(sh->line);	// needs to be NULL-terminated
		set_each_token_type(&token_list, -1);
		if (check_syntax(token_list, sh) != 0)
			continue;
		token_list = expand_tokens(token_list, sh, env_list);
		sh->ast = create_ast(token_list);
		free_token_list(token_list);
		dfs_ast(sh->ast, sh);
		free(sh->line);
		if (sh->ast)
			free_ast(sh->ast);
		g_signal_pid = 0;
	}
	free_struct(sh);
}

int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_minishell	*sh;

	(void)ac;
	(void)av;
	minishell_start();
	env_list = populate_env(env);
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

TODO:

([ ] wrong expansion with : echo $9HOME)
([ ] wrong expansion with : echo $$$$$$$$$$$HOME)	-> should not be handled

[ ] all free and all leaks
[ ] Norm and clean code:
	[ ] remove unused functions
	[ ] macros for all error strings
[ ] write proper Makefile
*/
