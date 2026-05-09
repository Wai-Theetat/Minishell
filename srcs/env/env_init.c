/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 21:19:15 by koonchevych       #+#    #+#             */
/*   Updated: 2026/05/09 13:43:51 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_env_new(char *key, char *value)
{
	t_env	*env_node;

	if (!key || !value)
		return (NULL);
	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = strdup(key);
	env_node->value = strdup(value);
	env_node->next = NULL;
	return (env_node);
}

void	ft_env_add_back(t_env **env, t_env *new_env_node)
{
	t_env	*current_env_node;

	if (!env || !new_env_node)
		return ;
	if (!*env)
	{
		*env = new_env_node;
		return ;
	}
	current_env_node = *env;
	while (current_env_node->next != NULL)
		current_env_node = current_env_node->next;
	current_env_node->next = new_env_node;
	return ;
}

t_env	*ft_env_from_line(char *line)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;

	eq = ft_strchr(line, '=');
	if (eq)
	{
		key = ft_substr(line, 0, eq - line);
		value = ft_strdup(eq + 1);
	}
	else
	{
		key = ft_strdup(line);
		value = ft_strdup("");
	}
	if (!key || !value)
		return (free(key), free(value), NULL);
	node = ft_env_new(key, value);
	free(key);
	free(value);
	return (node);
}

t_env	*ft_env_init(char **envp)
{
	t_env	*env;
	t_env	*node;
	size_t	i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		node = ft_env_from_line(envp[i]);
		if (!node)
			return (env);
		ft_env_add_back(&env, node);
		i++;
	}
	return (env);
}
