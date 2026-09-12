/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_order.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ENV_BUCKETS 1024

/*
** `env` prints the variables in the order the shell happens to store them.
** bash keeps them in a hash table (FNV-1a, 1024 buckets, newest entry first
** inside a bucket) and walks the buckets in ascending order, so its output
** is not the order of the inherited environ.  Reproducing that layout keeps
** `env` byte identical to bash - the order itself carries no meaning.
*/
static unsigned int	env_hash(const char *key)
{
	unsigned int	h;

	h = 2166136261U;
	while (*key)
	{
		h = h * 16777619U;
		h = h ^ (unsigned char)*key++;
	}
	return (h & (ENV_BUCKETS - 1));
}

static int	env_count(t_env *env)
{
	int	n;

	n = 0;
	while (env)
	{
		n++;
		env = env->next;
	}
	return (n);
}

static void	fill_array(t_env *env, t_env **arr)
{
	int	i;

	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
}

/*
** Within one bucket bash pushes new entries in front, so the inherited
** order has to be walked backwards.
*/
static t_env	*link_buckets(t_env **arr, int n)
{
	t_env	*head;
	int		bucket;
	int		i;

	head = NULL;
	bucket = -1;
	while (++bucket < ENV_BUCKETS)
	{
		i = n;
		while (--i >= 0)
		{
			if (env_hash(arr[i]->key) == (unsigned int)bucket)
			{
				arr[i]->next = NULL;
				ft_env_add_back(&head, arr[i]);
			}
		}
	}
	return (head);
}

t_env	*ft_env_reorder(t_env *env)
{
	t_env	**arr;
	t_env	*head;
	int		n;

	n = env_count(env);
	if (n < 2)
		return (env);
	arr = malloc(sizeof(t_env *) * n);
	if (!arr)
		return (env);
	fill_array(env, arr);
	head = link_buckets(arr, n);
	free(arr);
	return (head);
}
