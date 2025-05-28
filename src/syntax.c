/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:58:45 by gfontagn          #+#    #+#             */
/*   Updated: 2025/05/28 14:06:52 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	index_err_check(t_token **tkl, char has_c, char exp_f, int i)
{
	while (tkl[++i])
	{
		if (exp_f && !is_file(tkl[i]->type))
			return (i);
		else if (exp_f && is_file(tkl[i]->type))
		{
			exp_f = 0;
			has_c = 1;
		}
		else if (is_redirect(tkl[i]->type))
			exp_f = 1;
		else if (is_pipe(tkl[i]->type) || is_operator(tkl[i]->type))
		{
			if (!has_c)
				return (i);
			has_c = 0;
		}
		else if (is_command(tkl[i]->type))
			has_c = 1;
	}
	if (exp_f || !has_c)
		return (i);
	return (-1);
}

int	syntax_error(t_token *token, t_minishell *sh)
{
	char	*v;

	if (!token)
	{
		ft_dprintf(2, "minishell: " UNEXPECTED_NL);
		sh->last_exit = 2;
		return (2);
	}
	v = token->value;
	ft_dprintf(2, "minishell: " UNEXPECTED_S, v);
	sh->last_exit = 2;
	return (2);
}

int	check_syntax(t_token **tk_list, t_minishell *sh)
{
	int	has_c;
	int	exp_f;
	int	index;
	int	type;

	type = tk_list[0]->type;
	if (type == -1)
		return (0);
	index = check_first_last_token(tk_list);
	if (index != -1)
		return (syntax_error(tk_list[index], sh));
	exp_f = is_redirect(type);
	has_c = (is_command(type) || is_redirect(type));
	index = index_err_check(tk_list, has_c, exp_f, 0);
	if (index != -1)
		return (syntax_error(tk_list[index], sh));
	return (0);
}
