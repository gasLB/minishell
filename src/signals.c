/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:58:56 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/01 16:57:16 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_c(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_signal_pid == 0)
		rl_redisplay();
}

void	handle_bs(int sig)
{

}

void	set_signals(void)
{
	signal(SIGINT, &handle_c);
	signal(SIGQUIT, &handle_bs);
}

// I can't just ignore the ctrl+\ -> it sometimes need to core dump
// for the moment I just ignore my global
// global variable should just be the signal number (sig)
//
// 4 potential modes:
// - default mode -> in child process, just reset default handling 
// - interactive mode -> in readling, do all rl stuff for SIGINT
// - execution mode -> in parent, ignore everything
// - heredoc mode -> SIGINT prints newline and exits
//
// global should be used to know when received a signal
