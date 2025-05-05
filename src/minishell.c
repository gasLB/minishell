/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:27:04 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 15:28:53 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	only_space(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != ' ' && (*str)[i] != '\t')
			return (0);
		i++;
	}
	if (*str)
		free(*str);
	*str = NULL;
	return (1);
}

char	*read_one_line(t_minishell *sh)
{
	char	*line;

	dup2(sh->original_stdin, STDIN_FILENO);
	dup2(sh->original_stdout, STDOUT_FILENO);
	line = readline("\001\033[1;35m\002Minishell> \001\033[0m\002");
	if (line == NULL)
	{
		ft_printf("exit\n");
		free_struct(sh);
		exit(0);
	}
	return (line);
}

void	free_in_loop(t_token ***tkl, char **line, t_ast_node **ast)
{
	if (*tkl)
	{
		free_token_list(*tkl);
		*tkl = NULL;
	}
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	if (*ast)
	{
		free_ast(*ast);
		*ast = NULL;
	}
}

void	minishell(t_minishell *sh, t_env_list *env_list)
{
	t_token		**token_list;

	while (1)
	{
		sh->line = read_one_line(sh);
		add_history(sh->line);
		if (only_space(&(sh->line)))
			continue ;
		token_list = init_token_list(sh->line);
		set_each_token_type(&token_list, -1);
		if (check_syntax(token_list, sh) != 0)
		{
			free_in_loop(&token_list, &(sh->line), &(sh->ast));
			continue ;
		}
		token_list = expand_tokens(token_list, sh, env_list);
		sh->ast = create_ast(token_list);
		if (token_list)
			(free_token_list(token_list), token_list = NULL);
		dfs_ast(sh->ast, sh);
		free_in_loop(&token_list, &(sh->line), &(sh->ast));
		g_signal_pid = 0;
	}
	free_struct(sh);
}
