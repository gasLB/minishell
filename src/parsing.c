/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:13:29 by gfontagn          #+#    #+#             */
/*   Updated: 2025/04/17 21:48:19 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
// compile with -lreadline

char	set_quote_character(char c, char new)
{
	if ((c == 'D' && new == '"') || (c == 'S' && new == '\''))
		return ('N');
	else if (c == 'N' && new == '"')
		return ('D');
	else if (c == 'N' && new == '\'')
		return ('S');
	return (c);
}

char	*correct_substr(char *line, int start, int end)
{
	if (line[end] == '\0')
		return (ft_substr(line, start, end - start + 1));
	return (ft_substr(line, start, end - start));
}

void	append_token(t_token ***tk_list, t_token *token)
{
	t_token	**new_list;
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (*tk_list && (*tk_list)[count])
		count++;
	new_list = malloc(sizeof(t_token *) * (count + 2));
	if (!new_list)
		return;
	while (i < count)
	{
		new_list[i] = (*tk_list)[i];
		i++;
	}
	new_list[count] = token;
	new_list[count + 1] = NULL;
	free(*tk_list);
	*tk_list = new_list;
}

t_token	**new_populate_tokens(t_token **tk_list, char *line, int start, int end)
{
	char	quote;
	int	words;
	char	*sub;

	words = 0;
	quote = 'N';
	while (line[start])
	{
		while (line[start] && quote == 'N' && line[start] == ' ')
			start++;
		if (!line[start])
			break;
		end = start;
		while (line[++end])
		{
			quote = set_quote_character(quote, line[end]);
			if (quote == 'N' && line[end] == ' ')
				break;
		}
		sub = correct_substr(line, start, end);
		append_token(&tk_list, init_token(sub));
		start = end;
	}
	return (tk_list);
}

t_token	**init_token_list(char *line)
{
	t_token	**tk_list;

	tk_list = malloc(sizeof(t_token *));
	if (!tk_list)
		return (NULL);
	tk_list[0] = NULL;
	return (new_populate_tokens(tk_list, line, 0, 0));
}

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
	/*
	if (is_equal(token->value, "(") && is_equal(token->quote_mask, "N"))
		return (OPEN_PAR);
	if (is_equal(token->value, ")") && is_equal(token->quote_mask, "N"))
		return (CLOSE_PAR);
	*/
	return (-1);
}

// call with grp = -1
void	set_each_token_type(t_token ***tk_list_pt, int grp)
{
	int	i;
	int	type;

	i = 0;
	while (*tk_list_pt[i])
	{
		type = token_basic_type((*tk_list_pt)[i]);
		if (type > -1)
		{
			if (is_redirect(type))
				grp = REDIRECT;
			else
				grp = -1;
		}
		else
		{
			if (grp == REDIRECT)
				(type = FILE, grp = -1);
			else if (grp == COMMAND)
				type = ARG;
			else
				(type = CMD, grp = COMMAND);
		}
		((*tk_list_pt)[i]->type = type, i++);
	}
}

// Refaire main propre
int	main(int ac, char **av, char **env)
{
	t_env_list	*env_list;
	t_token		**tk_list;
	char *rl;

	(void)ac; (void)av; env_list = populate_env(env); while (1) {
		rl = readline("\e[35m\e[1mMinishell> \e[0m");
		if (rl)
			add_history(rl);
			token_list = init_token_list(rl);
			//if (is_syntax_correct(rl))
				parse(rl);
		break;
	}
	printf("%s\n", rl);
	free(rl);	
	return (0);
}

/*

---------------

The question is: must I check validity first or find each token type first?
-> I think I should first find each token's type. Because I need type information for checking syntax
This is more complicated than I thought. 
The quote status is important 

1) tokenization: raw + value + quote_mask + type

go through the line.
Keep account of current quote status
if q = N, space is considered a separator
	-> create a new token
if q = D or q = S, space is not a separator

2) check validity

3) expansion of $

4) creation of AST

---------------
2. tokenization: create a token_list with each lexeme
3. expansion: expand each token value
5. ast tree creation: create ast from tokens with the right precedence


I must free the ast before each readline but not the shell
*/

