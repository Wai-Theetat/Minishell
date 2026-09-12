/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_find(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
** bash quotes the value and escapes the characters that would change the
** meaning of the printed assignment.
*/
static void	print_value(char *value)
{
	int	i;

	ft_putstr_fd("=\"", STDOUT_FILENO);
	i = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\' || value[i] == '$'
			|| value[i] == '`')
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(value[i], STDOUT_FILENO);
		i++;
	}
	ft_putstr_fd("\"", STDOUT_FILENO);
}

static void	sort_keys(t_argv *keys)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < keys->n)
	{
		j = i + 1;
		while (j < keys->n)
		{
			if (ft_collate_cmp(keys->v[j], keys->v[i]) < 0)
			{
				tmp = keys->v[i];
				keys->v[i] = keys->v[j];
				keys->v[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	collect_keys(t_env *env, t_argv *keys)
{
	argv_init(keys);
	while (env)
	{
		argv_push(keys, env->key);
		env = env->next;
	}
	sort_keys(keys);
}

int	export_list(t_env *env)
{
	t_argv	keys;
	t_env	*node;
	int		i;

	collect_keys(env, &keys);
	i = 0;
	while (i < keys.n)
	{
		node = env_find(env, keys.v[i]);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(node->key, STDOUT_FILENO);
		if (node->has_value)
			print_value(node->value);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (0);
}
