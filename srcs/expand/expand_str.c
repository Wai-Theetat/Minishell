/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 12:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/11/19 12:22:11 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_len(const char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

static int	is_var_start(const char *str, int i)
{
	return (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'
			|| str[i + 1] == '?'));
}

static char	*get_var(const char *str, int *i, t_env *env, int exit_code)
{
	char	*key;
	char	*val;
	int		len;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_gc_itoa(exit_code));
	}
	len = var_len(str + *i);
	key = ft_gc_substr(str, *i, len);
	*i += len;
	val = ft_env_get(env, key);
	if (!val)
		return ("");
	return (val);
}

/* Expands $VAR and $? everywhere; quotes are kept as text (heredoc body). */
char	*expand_str(const char *str, t_env *env, int exit_code)
{
	char	*result;
	int		i;

	result = ft_gc_strjoin("", "");
	i = 0;
	while (str[i])
	{
		if (is_var_start(str, i))
			result = ft_gc_strjoin(result, get_var(str, &i, env, exit_code));
		else
			result = ft_gc_strjoin(result, ft_gc_substr(str, i++, 1));
	}
	return (result);
}

/* Expands a raw word: removes quotes, no $ expansion inside single quotes. */
char	*expand_word(const char *str, t_env *env, int exit_code)
{
	char	*result;
	char	quote;
	int		i;

	result = ft_gc_strjoin("", "");
	quote = 0;
	i = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i++];
		else if (quote && str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (quote != '\'' && is_var_start(str, i))
			result = ft_gc_strjoin(result, get_var(str, &i, env, exit_code));
		else
			result = ft_gc_strjoin(result, ft_gc_substr(str, i++, 1));
	}
	return (result);
}
