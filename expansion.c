/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:28:51 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/17 19:52:29 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

// this function is used with: echo

// We first need a global function that takes as input the list of arguments
// Wildcards are res AFTER variables

/*
 
1) VARIABLES EXPANSION

$: nothing.

$?: The exit status of the most recently executed command
$ABCab_32 : valid variable name -> search for it in env. If not found, returns nothing
$PATH-abc: valid until '-' -> search for PATH and appends '-abc' next to it
$HOME$?$-$!: valid, each block attached to each other. If HOME is not found, return just $?$-$!

\$USER: just the string '$USER' -> '\' preserve litteral meaning of next character
'\'$HOME: \/home/user -> escape the \ with single quotes
'$VAR': return litteral string $VAR

'"$?"': "$?"
'"'$?'"': "0"
"'$?'": '0'
""'$?'"": $?

2) WILDCARDS

*: any string of characters
ab*: all files starting with 'ab' (including 'ab' itslef)
*slash*: all files in subdirectories
$CWD*: all files in CWD
"*": wildcard are treated as litterals inside double quotes

*/

// just after the $ -> check if special char among $?!0#*-

int	try_expand(char **to_expand, char *src, t_env_list *env, int l_e_s)
{
	int	i;
	char	*expanded;

	i = 0;
	expanded = NULL;
	if (!src[i])
		return (0);
	if (src[i] == '?')
	{
		*to_expand = append_str(*to_expand, ft_itoa(l_e_s));
		return (1);
	}
	if (!(ft_is_alpha(src[i]) || (src[i] == '_')))
		return (0);
	while (str[i] && (ft_is_alnum(str[i]) || (src[i] == '-'))) // warning: check the ennv variable as late as possible
		expanded = append_char(expanded, str[i++]);
	if (expanded)
	{
		expanded = append_char(expanded, '\0');
		*to_expand = append_str(*to_expand, ft_getenv(expanded)); // warning: getenv could be NULL
		free(expanded);
	}
	return (i);
}

// first version without taking q_mask into account. str is the real str without any quotes
char	*expand_variable_basic(char *str, char *q_mask, t_env_list *env, int l_e_s)
{
	char	*expanded;
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			i += try_expand(&expanded, str + i, l_e_s); //pass expanded as a pointer to change it
		else if (str[i])
			(expanded = append_char(expanded, str[i]), i++);
	}
}

// We define 2 types of groupings inside each variable: litteral groupings and expandables groupings
// Expandable groupings have a valid name. 
//// In bash, Variable names must start with a letter or underscore, followed by any combination of letters, numbers, or underscores
// They refer to an environment variable (or special code) that may exist or not
//
// Litteral groupings are just treated as character strings
// 
//
//	VARIABLE	abc$HOME-def$PATHabc7$?$+g$	
//
//			abc $HOME -def $PATHabc7 $? $+g $
//			--- ----- ---- --------- -- --- -
//	TYPE:		 L    E     L      E     E   L  L
//	EXIST:		      Y            N     Y     
//
//	RESULT:		abc /home/gfontagn/ -def 0 $+g $
//
//	(concatenated)	abc/home/gfontagn/-def0$+g$
//
//
// IMPLEMENTATION
// 
// We iterate on initial value. Current grouping is treated as litteral until we find a $:
// 	- we write the current grouping.
// 	- if $-grouping is not valid (we know by character next to $), treated as litteral until we find a $
// 	- if $-grouping is valid, we try to expand it:
// 		- if $-grouping doesn't refer to existing variable, we skip it
// 		- if $-grouping refers to existing variable, we write it
// 
//

int	is_good_c(char c)
{
	if (c == NULL)
		return (0);
	return (ft_isalpha(c) || c == '_' || c == '?');
}

int	is_valid_inside(char c)
{
	return (c != '$' && (ft_isalnum(c) || c == '_'));
}

// now implement q_mask and  handling for cases like $HOME/something 

char	*expand_variables_basic(char *str, char *q_mask, t_minishell *sh)
{
	char	*res;
	int	begin;
	int	i;

	i = 0;
	begin = 0;
	res = NULL;
	while (str[i])
	{
		while (str[i] && ((str[i] != '$') || (!is_good_c(str[i + 1]))))		// while being litteral
			i++;
		res = append_str(res, ft_substr(str, begin, i + 1));			// from here, either NULL or valid $-grouping  
		if (!str[i++])
			break;
		if (str[i] == '?')
			(res = append_str(res, ft_itoa(sh->last_exit)), i++);
		begin = i;								// begin and i and the start of the new group (without $)
		while (str[i] && is_valid_inside(str[i]))
			i++;
		res = append_str(res, \
		   ft_strdup(ft_getenv(ft_substr(str, begin, i + 1), env)));
	}
	return (res);
}
