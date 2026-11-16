/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/11/16 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	key_cmp(const char *a, const char *b)
{
	return (ft_strncmp(a, b, ft_strlen(a) + 1));
}

/* Smallest key strictly after prev (prev NULL = smallest key overall). */
static t_env	*next_in_order(t_env *env, t_env *prev)
{
	t_env	*best;

	best = NULL;
	while (env)
	{
		if ((!prev || key_cmp(env->key, prev->key) > 0)
			&& (!best || key_cmp(env->key, best->key) < 0))
			best = env;
		env = env->next;
	}
	return (best);
}

/* `export` with no arguments: list every variable sorted by name. */
int	print_export(t_env *env)
{
	t_env	*cur;

	cur = next_in_order(env, NULL);
	while (cur)
	{
		ft_printf("declare -x %s=\"%s\"\n", cur->key, cur->value);
		cur = next_in_order(env, cur);
	}
	return (0);
}
