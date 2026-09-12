/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:10:51 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
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
	if (len == 0)
		return (ft_gc_strjoin("$", ""));
	key = ft_gc_malloc(len + 1);
	ft_strlcpy(key, str + *i, len + 1);
	*i += len;
	val = ft_env_get(env, key);
	if (!val)
		return (ft_gc_strjoin("", ""));
	return (val);
}

static int	is_var_start(const char *str, const char *mask, int i)
{
	if (str[i] != '$' || mask[i] == 2)
		return (0);
	if (!str[i + 1])
		return (0);
	return (ft_isalpha(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?');
}

/*
** Expands $VAR / $? while keeping track of what may still be word-split and
** globbed afterwards: characters that came from a quoted section - and the
** text of a variable that was referenced inside double quotes - are marked
** protected (mask byte 1).
*/
t_wstr	expand_word(const char *str, const char *mask, t_env *env,
		int exit_code)
{
	t_wstr	w;
	char	*val;
	int		i;
	int		start;

	wstr_init(&w);
	i = 0;
	while (str && str[i])
	{
		if (is_var_start(str, mask, i))
		{
			start = i;
			val = get_var(str, &i, env, exit_code);
			wstr_addstr(&w, val, mask[start] == 1);
		}
		else
		{
			wstr_addc(&w, str[i], mask[i] != 0);
			i++;
		}
	}
	return (w);
}

char	*expand_str(const char *str, t_env *env, int exit_code)
{
	char	*result;
	char	*tmp;
	char	buf[2];
	int		i;

	result = ft_gc_strjoin("", "");
	i = 0;
	buf[1] = '\0';
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1]
			&& (ft_isalpha(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?'))
		{
			tmp = get_var(str, &i, env, exit_code);
			result = ft_gc_strjoin(result, tmp);
		}
		else
		{
			buf[0] = str[i++];
			result = ft_gc_strjoin(result, buf);
		}
	}
	return (result);
}
