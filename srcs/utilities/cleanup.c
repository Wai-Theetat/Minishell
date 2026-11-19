/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theetat <theetat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 09:30:00 by theetat           #+#    #+#             */
/*   Updated: 2026/11/19 09:30:00 by theetat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*get_shell(t_shell *set)
{
	static t_shell	*shell = NULL;

	if (set)
		shell = set;
	return (shell);
}

void	ft_env_free(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

void	ft_free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
}

void	clean_exit(int code)
{
	t_shell	*shell;

	shell = get_shell(NULL);
	if (shell)
	{
		ft_env_free(shell->envp);
		shell->envp = NULL;
		free(shell->input);
		shell->input = NULL;
		if (shell->saved_in != -1)
			close(shell->saved_in);
		if (shell->saved_out != -1)
			close(shell->saved_out);
		shell->saved_in = -1;
		shell->saved_out = -1;
	}
	ft_gc_clear();
	rl_clear_history();
	exit(code);
}
