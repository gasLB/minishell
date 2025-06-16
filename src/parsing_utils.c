/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:36:03 by gfontagn          #+#    #+#             */
/*   Updated: 2025/06/16 14:13:54 by seetwoo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_basic_type(t_token *token)
{
	if (is_equal(token->value, "<") && is_equal(token->quote_mask, "N"))
		return (IN);
	if (is_equal(token->value, "<<") && is_equal(token->quote_mask, "NN"))
		return (HD);
	if (is_equal(token->value, ">") && is_equal(token->quote_mask, "N"))
		return (TRUNC);
	if (is_equal(token->value, ">>") && is_equal(token->quote_mask, "NN"))
		return (APPEND);
	if (is_equal(token->value, "|") && is_equal(token->quote_mask, "N"))
		return (PIPE);
	if (is_equal(token->value, "&&") && is_equal(token->quote_mask, "NN"))
		return (AND);
	if (is_equal(token->value, "||") && is_equal(token->quote_mask, "NN"))
		return (OR);
	return (-1);
}

int	end_op(char *line, int end, char quote)
{
	char	le;

	if (!line[end])
		return (0);
	le = line[end];
	if (quote == 'N' && (le == '<' || le == '>' || le == '|' || le == '&'))
	{
		if (line[end + 1]
			&& ((line[end] == '<' && line[end + 1] == '<')
				|| (line[end] == '>' && line[end + 1] == '>')
				|| (line[end] == '|' && line[end + 1] == '|')
				|| (line[end] == '&' && line[end + 1] == '&')))
		{
			return (2);
		}
		else
			return (1);
	}
	return (0);
}

char	*correct_substr(char *line, int start, int end)
{
	if (line[end] == '\0')
		return (ft_substr(line, start, end - start + 1));
	return (ft_substr(line, start, end - start));
}

int	subshell_len(char *line, int end)
{
	int	paren;

	paren = 1;
	end++;
	while (1)
	{
		if (!line[end] || paren == 0)
			break ;
		if (line[end] == '(')
			paren++;
		if (line[end] == ')')
			paren--;
		end++;
	}
	return (end);
}

int	update_token_end(char *line, char quote, int end)
{
	if (line[end] == '(' && quote == 'N')
		return (subshell_len(line, end));
	while (line[end])
	{
		quote = get_quote_character(quote, line[end], end, NULL);
		if (quote == 'N'
			&& (line[end] == ' ' || line[end] == '(' || end_op(line, end, quote)))
			break ;
		end++;
	}
	return (end);
}
