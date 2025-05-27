/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:15:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/05 19:24:51 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int	compare_line(char *line, char *lim)
{
	if (ft_strlen(line) - 1 == ft_strlen(lim))
		return (ft_strncmp(line, lim, ft_strlen(lim)));
	return (1);
}

int	get_next_line_input(char **line, t_minishell *sh)
{
	int		i;
	char	c;
	char	*buffer;

	(printf_fd(2, "> "), i = 0);
	buffer = malloc(BUFFER_SIZE * sizeof(char));
	if (!buffer)
		return (0);
	if (read(sh->original_stdin, &c, 1) <= 0)
		return (free(buffer), 0);
	while (c != '\n' && c != '\0')
	{
		buffer[i++] = c;
		if (read(sh->original_stdin, &c, 1) <= 0)
		{
			buffer[i++] = '\n';
			buffer[i] = '\0';
			*line = buffer;
			return (1);
		}
	}
	buffer[i++] = '\n';
	buffer[i] = '\0';
	*line = buffer;
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

void	put_line_hd(char *line, int fd[2], t_minishell *sh, int in_status)
{
	t_token		*tk;
	t_env_list	*env;

	env = sh->env_list;
	tk = malloc(sizeof(t_token));
	if (!tk)
		return ;
	tk->value = ft_strdup(line);
	if (!tk->value)
		return ;
	tk->quote_mask = fill_with_char(ft_strlen(line), 'N');
	if (!tk->quote_mask)
		return ;
	tk->transition_mask = fill_with_char(ft_strlen(line), 'n');
	if (!tk->transition_mask)
		return ;
	if (in_status == HD)
		ft_putstr_fd(expand_variable(tk, sh, env, 0), fd[1]);
	else if (in_status == HDQ)
		ft_putstr_fd(ft_strdup(line), fd[1]);
	free_token(tk);
	if (line)
		free(line);
}

int	here_doc(char *lim, t_minishell *sh, int in_status)
{
	int		fd[2];
	int		pid;
	char	*line;

	line = NULL;
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		while (get_next_line_input(&line, sh))
		{
			if (compare_line(line, lim) == 0)
				break ;
			put_line_hd(line, fd, sh, in_status);
			line = NULL;
		}
		(close_pipe_safely(&(fd[1])), close_all_pipes(sh), free_struct(sh));
		if (line)
			free(line);
		exit(EXIT_SUCCESS);
	}
	(close_pipe_safely(&(fd[1])), waitpid(pid, NULL, 0));
	return (fd[0]);
}
