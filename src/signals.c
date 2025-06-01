/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:58:56 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/30 20:46:41 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_interactive(int sig)
{
	g_signal = sig; 
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint_execution(int sig)
{
	g_signal = sig;
	ft_printf("\n");
}

void	handle_sigint_heredoc(int sig)
{
	g_signal = sig;
}

void	handle_sigquit_execution(int sig)
{
	g_signal = sig;
	ft_printf("Quit (core dumped)\n");
}

void	set_signals_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	set_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_interactive(void)
{
	signal(SIGINT, &handle_sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_execution(void)
{
	signal(SIGINT, &handle_sigint_execution);
	signal(SIGQUIT, &handle_sigquit_execution);
}

void	set_signals_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL); // not sure -> core dumped and last_exit = 131
}

// I can't just ignore the ctrl+\ -> it sometimes need to core dump
// for the moment I just ignore my global
// global variable should just be the signal number (sig)
//
// 4 potential modes:
// - default mode -> in child process during execution, just reset default handling 
// In external only or also in 
// - interactive mode -> in readling, reset readline() stuff for SIGINT
// - execution mode -> in parent, ignore everything
// - heredoc mode -> SIGINT prints newline and exits
//
// global should be used to know when received a signal to 
// - update last_exit
//
// -> still need to handle Ctrl+D
//
// -> reset g_signal to 0 when needed
// what is 131?
//
// FOR heredoc:
// Should I use SA_RESTART in HEREDOC?
// -> I want to exit all processes 
// 
// Should I wait all pids before heredoc or before the pipe?
//
// ------- TESTS
//
//							Behav  leaks  exit status
// In READLINE: 
//				ctrl+C :		OK | OK | OK
//				ctrl+D:			OK | OK | OK
//				ctrl+\:			OK | OK | OK
//
// In WAITING (cat):
//				ctrl+C:			OK | OK | OK
//				ctrl+D:			OK | OK | OK 
//				ctrl+\:			OK | OK | OK 
//
// In HEREDOC:
//				ctrl+C:			OK | OK | OK
//				ctrl+D:			~~ | OK | OK (spam touche)
//				ctrl+\:			
//
///Minishell> << LIM
/*
> ^C

Minishell> << LIM
> ^C

Minishell> << LIM | echo prou
> ^C
Minishell> 

*/
