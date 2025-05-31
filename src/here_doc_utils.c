/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:26:39 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/30 20:50:07 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_func.h"

int	compare_line(char *line, char *lim)
{
	if (ft_strlen(line) - 1 == ft_strlen(lim))
		return (ft_strncmp(line, lim, ft_strlen(lim)));
	return (1);
}

char	*fill_with_char(int n, char c)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc((n + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i < n)
	{
		res[i] = c;
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*expand_line(char *line, int stat, t_minishell *sh)
{
	char	*res;
	t_token	*tk;

	res = NULL;
	if (!line)
		return (NULL);
	tk = malloc(sizeof(t_token));
	if (!tk)
		return (NULL);
	tk->value = ft_strdup(line);
	if (!tk->value)
		return (NULL);
	tk->quote_mask = fill_with_char(ft_strlen(line), 'N');
	if (!tk->quote_mask)
		return (NULL);
	tk->transition_mask = fill_with_char(ft_strlen(line), 'n');
	if (!tk->transition_mask)
		return (NULL);
	if (stat == HD)
		res = expand_variable(tk, sh, sh->env_list, 0);
	else if (stat == HDQ)
		res = ft_strdup(line);
	free_token(tk);
	if (line)
		(free(line), line = NULL);
	return (res);
}

void	exit_heredoc_signal(int fd[2], char **args, char *str, t_minishell *sh)
{
	if (args)
		free_str_list(args);
	if (str)
		free(str);
	str = NULL;
	close_pipe_safely(&(fd[1]));
	close_all_pipes(sh);
	free_struct(sh);
	g_signal = 0;
	exit(130);
}

void	exit_heredoc_lim(int fd[2], char *line, char *str, t_minishell *sh)
{
	//printf_fd(2, "exit lim\n");
	if (str)
	{
		ft_putstr_fd(str, fd[1]);
		free(str);
		str = NULL;
	}
	if (line)
		free(line);
	line = NULL;
	close_pipe_safely(&(fd[1]));
	close_all_pipes(sh);
	free_struct(sh);
	exit(EXIT_SUCCESS);
}
