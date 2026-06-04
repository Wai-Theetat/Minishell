/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 13:41:16 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/04 15:45:44 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_env_to_char(t_env *env)
{
	char	**arr;
	t_env	*tmp;
	size_t	count;
	size_t	i;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		arr[i++] = ft_strjoin(ft_strjoin(tmp->key, "="), tmp->value);
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	*ft_env_get(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	ft_env_set(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	ft_env_add_back(env, ft_env_new(key, value));
}

void	ft_env_unset(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = *env;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

// test env_list
void	ft_env_list(t_env **envp)
{
	t_env	*current_node;

	if (!envp)
		return ;
	current_node = *envp;
	while (current_node)
	{
		printf("%s:%s", current_node->key, current_node->value);
		if (current_node->next)
			printf(" -> ");
		current_node = current_node->next;
	}
	printf("\n");
	return ;
}
