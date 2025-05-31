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
#include <sys/wait.h>

int	read_and_check_signal(char *c, t_minishell *sh)
{
	int	read_res;

	if (g_signal == SIGINT)
		return (-1);
	read_res = read(sh->original_stdin, c, 1);
	if (read_res == 0)
		printf_fd(2, "bash: warning: here-document at line .. delimited by end-of-file (wanted `LIM')\n");
	if (g_signal == SIGINT)
		return (-1);
	return (read_res);
}

int	update_line_buffer(char **line, char *buffer, int i)
{
	buffer[i++] = '\n';
	buffer[i] = '\0';
	*line = buffer;
	return (1);
}

int	get_next_line_input(char **line, t_minishell *sh)
{
	int		i;
	char	c;
	char	*buffer;

	printf_fd(2, "> ");
	i = 0;
	buffer = malloc(BUFFER_SIZE * sizeof(char));
	if (!buffer)
		return (0);
	*line = NULL;
	if (read_and_check_signal(&c, sh) <= 0)
		return (free(buffer), 0);
	while (c != '\n' && c != '\0')
	{
		buffer[i++] = c;
		if (read_and_check_signal(&c, sh) <= 0)
		{
			if (g_signal != SIGINT)
				return (update_line_buffer(line, buffer, i));
			return (free(buffer), 0);	
		}
	}
	return (update_line_buffer(line, buffer, i));
}

int	wait_for_heredoc_child(int fd[2], char *str, int pid, t_minishell *sh)
{
	int	status;
	int	exit_code;

	if (str)
		free(str);
	if (pid <= 0)
        return (-1);
	close_pipe_safely(&(fd[1]));
	waitpid(pid, &status, 0);
	exit_code = WEXITSTATUS(status);
	if (exit_code == 130)
	{
		printf_fd(2, "\n");
		sh->heredoc_interrupted = 1;
		sh->last_exit = 130;
		set_signals_interactive();
		return (-2);
	}
	sh->last_exit = exit_code;
	set_signals_execution();
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
		set_signals_heredoc();
		close_pipe_safely(&(fd[0]));
		while (g_signal != SIGINT && get_next_line_input(&line, sh))
		{
			if (g_signal == SIGINT)
				exit_heredoc_signal(fd, args, str, sh);
			if (compare_line(line, lim) == 0)
				break ;
			if (line)
				str = append_line_to_str(line, str, in_status, sh);
		}
		if (g_signal == SIGINT)
			exit_heredoc_signal(fd, args, str, sh);
		(free_str_list(args), exit_heredoc_lim(fd, line, str, sh));
		return (-1);
	}
	else
		return (wait_for_heredoc_child(fd, str, pid, sh));
}

// THe real bash behavior doesn't execute anytything BEFORE or AFTER heredoc interruption
// I think we could just avoid to execute everything AFTER interrupt
// The reason is that it is only possible to check that with &&.. Which is in bonus only 
// remove the sh->heredoc_interrupt
//
//
// ERROR: 
// should I reset g_signal ?
// should I use SA_RESTART ?
