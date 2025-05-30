/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:15:41 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/30 20:50:06 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "minishell_func.h"

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

int	wait_for_heredoc_child(int fd[2], char *str, int pid, t_minishell *sh)
{
	int	status;

	if (str)
		free(str);
	close_pipe_safely(&(fd[1]));
	waitpid(pid, &status, 0);
	sh->last_exit = status;
	set_signals_execution();
	printf_fd(2, "g_signal in wait_for_heredoc: %d\n", g_signal);
	if (g_signal != 0)
	{
		g_signal = 0;
		return (-1);
	}
	return (fd[0]);
}

char	*append_line_to_str(char *line, char *str, int stat, t_minishell *sh)
{
	char	*new_line;

	new_line = expand_line(line, stat, sh);
	if (!new_line)
		return (str);
	return (append_str(str, new_line));
}

int	init_heredoc_values(char **line, char **str, int fd[2])
{

	set_signals_heredoc();
	*line = NULL;
	*str = init_str();
	if (!(*str))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	return (0);
}

int	here_doc(char *lim, char **args, int in_status, t_minishell *sh)
{
	int		fd[2];
	int		pid;
	char	*line;
	char	*str;

	if (init_heredoc_values(&line, &str, fd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close_pipe_safely(&(fd[0]));
		while (get_next_line_input(&line, sh))
		{
			if (g_signal == SIGINT)
				(free_str_list(args), exit_heredoc_signal(fd, line, str, sh));
			if (compare_line(line, lim) == 0)
				break ;
			str = append_line_to_str(line, str, in_status, sh);
		}
		(free_str_list(args), exit_heredoc_lim(fd, line, str, sh));
	}
	return (wait_for_heredoc_child(fd, str, pid, sh));
}

// handle the case with EOF (CTRL+D)
