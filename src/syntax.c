/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:58:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/17 19:43:09 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>

int	check_first_last_token(t_token **tk_list)
{
	int	i;
	int	type;

	i = 0;
	type = tk_list[0]->type;
	if (is_pipe(type) || is_operator(type))
		return (i);
	while (tk_list[i])
		i++;
	type = tk_list[i - 1]->type;
	if (is_pipe(type) || is_operator(type) || is_redirect(type))
		return (i);
	return (-1);
}

int	index_err_check(t_token **tk_list, char has_c, char exp_c, char exp_f)
{
	int	i;
	int	type;

	i = 0;
	while (tk_list[++i])
	{
		type = tk_list[i]->type;
		if (exp_f && !is_file(type))
			return (i);
		else if (exp_f)
			exp_f = 0;
		else if (is_redirect(type))
			exp_f = 1;
		else if (is_pipe(type) || is_operator(type))
		{
			if (!has_c)
				return (i);
			(exp_c = 1, has_c = 0);
		}
		else if (is_command(type))
			(has_c = 1, exp_c = 0);
	}
	if (exp_f || exp_c)
		return (i);
	return (-1);
}

int	syntax_error(t_token *token, t_minishell *sh)
{
	char	*v;

	if (!token)
	{
		printf_fd(2, "minishell: syntax error near unexpected token `newline'");
		sh->last_exit = 2;
		return (2);
	}
	v = token->value;
	printf_fd(2, "minishell: syntax error near unexpected token %s", v);
	sh->last_exit = 2;
	return (2);
}

int	check_syntax(t_token **tk_list, t_minishell *sh)
{
	int	has_c;
	int	exp_c;
	int	exp_f;
	int	index;

	if (!tk_list[0])
		return (0);
	index = check_first_last_token(tk_list);
	if (index != -1)
		return (syntax_error(tk_list[index], sh));
	exp_c = 0;
	exp_f = is_redirect(tk_list[0]->type);
	has_c = is_command(tk_list[0]->type) || exp_f;
	index = index_err_check(tk_list, has_c, exp_c, exp_f);
	if (index != -1)
		return (syntax_error(tk_list[index], sh));
	return (0);
}

/*

When syntax error encountered, it immediately stops parsing and executing and exits

! The shell is given as input to set last_exit_status to 2

the check_syntax retunrs an int, set the last_exit if err is found and prints err message on stderr

----------

CHECKER

Do I need precedence climbing to check the correctness of syntax?

cmd | cmd arg > file && cmd

RULES

1. Command structure: Commands should start with a Command token (C) followed by optionnal arguments (A) and optional redirections (R)
2. Pipe structure: Pipes (P) must have commands on both sides (or null-cmd)
3. Operator structure: Operators (O) must have commands on both sides (or null_cmd)
4. Redirection structure: Redirections (R) must be followed by file (F)
5. Valid start/end: Lines can't start with P or O, and can't end with P, O, or R

ERRORS

(1. Unclosed parenthesis) to do later...
2. Dangling pipes or redirections (|, >, >>, <, << with nothing following)
3. Invalid redirection (missing files)
4. Emtpy commands between pipes
5. Consecutive operators without commands between them


EXAMPLES

# Dangling pipe
ls -la |

# Invalid redirection
cat >

# Invalid operator
ls -la && 

SPECIAL CASES

pipes should have at least one command to the right and to the left

what about this:
> caca 
it creates the file caca and is valid
where should I handle those emtpy commands? When creating the AST?
Or before when assigning types to token?
*/

