/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:10:51 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/31 02:30:34 by tdharmar         ###   ########.fr       */
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

static char	*get_var(const char *str, int *i, t_env *env)
{
	char	*key;
	char	*val;
	int		len;

	(*i)++;
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

static char	*expand_str(const char *str, t_env *env)
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
			&& (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			tmp = get_var(str, &i, env);
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

static void	expand_cmd(t_cmd *cmd, t_env *env)
{
	int	i;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (cmd->arg_quotes[i] != '\'')
			cmd->args[i] = expand_str(cmd->args[i], env);
		i++;
	}
	if (cmd->infile)
		cmd->infile = expand_str(cmd->infile, env);
	if (cmd->outfile)
		cmd->outfile = expand_str(cmd->outfile, env);
}

void	ft_expand(t_cmd *cmds, t_env *env)
{
	while (cmds)
	{
		expand_cmd(cmds, env);
		cmds = cmds->next;
	}
}
