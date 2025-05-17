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

void	close_all_pipes(t_minishell *sh)
{
	int	i;

	i = 0;
	while (i < sh->pipe_count)
	{
		close_pipe_safely(&(sh->pipe_fds[i]));
		i++;
	}
}

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

	i = 0;
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

void	put_line_hd(char *line, int fd[2], t_minishell *sh, t_env_list *env)
{
	t_token	*tk;

	tk = init_token(line);
	if (!tk)
		return ;
	tk->expanded_value = expand_variable(tk, sh, env, 0);
	ft_putstr_fd(tk->expanded_value, fd[1]);
	if (tk->value)
		free(tk->value);
	if (tk->quote_mask)
		free(tk->quote_mask);
	if (tk->transition_mask)
		free(tk->transition_mask);
	if (tk->expanded_value)
		free(tk->expanded_value);
	if (tk)
		free(tk);
}

int	here_doc(char *lim, t_minishell *sh, t_env_list *env)
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
			put_line_hd(line, fd, sh, env);
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
