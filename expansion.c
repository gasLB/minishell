/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfontagn <gfontagn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:28:51 by gfontagn          #+#    #+#             */
/*   Updated: 2025/03/20 18:00:48 by gfontagn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftprintf/libft/libft.h"
#include "../libftprintf/include/ft_printf_bonus.h"
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	is_expandable(char *str, char *q_mask, int i)
{
	char	next;

	next = 0;
	if (q_mask[i] == 'S')
		return (0);
	else if (str[i] == '$')
	{
		next = str[i + 1];
		if (next == '\0')	
			return (0);
		return (ft_isalpha(next) || next == '_' || next == '?');
	}
	return (0);
}

int	translation(char **res, t_token *tk, int i)
{
	int	begin;

	begin = i;
	while (tk->quote_mask[i] == 'S' || tk->quote_mask[i] == 'D')
		i++;
	*res = append_str(*res, ft_substr(tk->value, begin, i + 1));
	return (i);
}

int	is_valid_char_inside(char c_str, char c_q)
{
	if (!c_str || !c_q)
		return (0);
	else if (c_str != '$' && (ft_isalnum(c_str) || c_str == '_'))
	{
		if (c_q == 'N')
			return (1);
	}
	return (0);
}

int	look_for_env_variable(char **res, t_token *tk, int i, t_env_list *env)
{
	int	begin;

	begin = i;
	while (is_valid_char_inside(str[i], q_mask[i]))
		i++;
	res = append_str(res, \
		ft_strdup(ft_getenv(ft_substr(tk->value, begin, i + 1), env)));
	return (i);
}

char	*expand_variable(t_token *tk, t_minishell *sh)
{
	char	*res;
	int	begin;
	int	i;

	(i = 0, begin = 0, res = init_str());
	if (!res)
		return(NULL);
	i = handle_tilde(&res, tk, sh->env);
	while (tk->value[i])
	{
		while (!is_expandable(tk->value, tk->quote_mask, i))
			i++;
		res = append_str(res, ft_substr(tk->value, begin, i + 1));
		if (!tk->value[i++])
			break;
		if (tk->quote_mask[i] == 'S' || tk->quote_mask[i] == 'D')
			i = translation(&res, tk, i);
		else if (str[i] == '?')
			(res = append_str(res, ft_itoa(sh->last_exit)), i++);
		else
			i = look_for_env_variable(&res, tk, i, sh->env);
	}
	return (res);
}

// I will focus on wildcards later
// what's up with the numbers? 
// echo $987
// -> 87
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



just after the $ -> check if special char among $?!0#*-

We define 2 types of groupings inside each variable: litteral groupings and expandables groupings
Expandable groupings have a valid name. 
In bash, Variable names must start with a letter or underscore, followed by any combination of letters, numbers, or underscores
They refer to an environment variable (or special code) that may exist or not

Litteral groupings are just treated as character strings


       VARIABLE	abc$HOME-def$PATHabc7$?$+g$	

       		abc $HOME -def $PATHabc7 $? $+g $
       		--- ----- ---- --------- -- --- -
       TYPE:		 L    E     L      E     E   L  L
       EXIST:		      Y            N     Y     

       RESULT:		abc /home/gfontagn -def 0 $+g $

       (concatenated)	abc/home/gfontagn-def0$+g$


IMPLEMENTATION

We iterate on initial value. Current grouping is treated as litteral until we find a $:
	- we write the current grouping.
	- if $-grouping is not valid (we know by character next to $), treated as litteral until we find a $
	- if $-grouping is valid, we try to expand it:
		- if $-grouping doesn't refer to existing variable, we skip it
		- if $-grouping refers to existing variable, we write it


first, I will test this function with echo without taking into account quotes
now implement q_mask and  handling for cases like $HOME/something 

MASK:

B (backslash)
S (single quote)
D (double quote)
N (no quote)

echo $string		:	NNNNNNN
->
echo $"string"		:	NDDDDDD
-> string
echo $'string'		:	NSSSSSS
-> string
echo $""		:	N	(!special case: empty str after $)
-> 
echo " $ "		:	DDD
->  $  
echo ab"cde"		:	NNDDD
->abcde
echo "ab""cde"		:	DDDDD
-> abcde
echo 'ab'cde		:	SSNNN
->abcde
echo 'ab'"cde"		:	SSDDD
->abcde
echo ''ab''		:	NN
-> ab
echo ""ab""		:	NN
-> ab
echo "'ab'"		:	DDDD
->'ab'
echo '"ab"'		:	SSSS
->"ab"
echo ""'ab'""		:	SS
-> ab
echo ''"ab"''		:	DD
-> ab
echo ""''ab''""	:	NN
-> ab
echo '"'ab'"'		:	SNNS
->"ab"
echo "'"ab"'"		:	DNND
->'ab'
echo '"'"ab"'"'	:	SDDS
-> "ab"
echo "'"'ab'"'"	:	DSSD
-> 'ab'
echo abc\d		:	NNNBN
-> abcd
echo 'abc\d'		:	SSSSS
-> abc\d
echo "abc\d"		:	DDDDD
-> abc\d
echo \$HOME		:	BNNNN
-> $HOME
echo "$"HOME		:	DNNNN
-> $HOME
echo "$HO"ME		:	DDDNN
-> ME
echo $'\?'		:	NSS
-> ?
echo ~
-> /home/gfontagn
echo "~"
-> ~
echo '~'
-> ~
echo ~a
-> ~a
echo a~
-> a~
echo ~/
-> /home/gfontagn/
echo ~/abc
-> /home/gfontagn/abc
echo ~/~
-> /home/gfontagn/~
echo ~$HOME
-> ~/home/gfontagn
echo ~/"abcd"
-> /home/gfontagn/rere
echo ~/$HOME
-> /home/gfontagn//home/gfontagn
echo *
-> a.c b.cc.c text.md ...
echo exp*
-> expansion.c expansion_utils.c
echo "exp"*'c'
-> expansion.c expansion_utils.c
echo exp*prout
-> exp*prout
echo $HOME*prout
-> /home/gfontagn*prout
echo *exp$HOME
-> *exp/home/gfontagn



 Special cases:
 Inside double quotes, backslash retains its special meaning ONLY for certain characters:

    \" - escapes a double quote
    \\ - escapes a backslash
    \$ - escapes a dollar sign
    ``` - escapes a backtick
    \! - escapes history expansion (when enabled)
    \newline - line continuation


*/
// What to do if not valid sequence? ( like unclosed quote)
// fuck it for now
//

