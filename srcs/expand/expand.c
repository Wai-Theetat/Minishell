/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:10:51 by tdharmar          #+#    #+#             */
/*   Updated: 2026/11/19 12:22:14 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_empty_args(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args && cmd->args[i])
	{
		if (cmd->arg_quotes[i] == 0 && cmd->args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		cmd->args[j] = cmd->args[i];
		cmd->arg_quotes[j] = cmd->arg_quotes[i];
		i++;
		j++;
	}
	if (cmd->args)
		cmd->args[j] = NULL;
}

static void	expand_cmd(t_cmd *cmd, t_env *env, int exit_code)
{
	int		i;
	t_token	*r;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		cmd->args[i] = expand_word(cmd->args[i], env, exit_code);
		i++;
	}
	remove_empty_args(cmd);
	r = cmd->redirs;
	while (r)
	{
		if (r->type != TOKEN_HEREDOC)
			r->value = expand_word(r->value, env, exit_code);
		r->ambiguous = is_ambiguous_redir(r);
		r = r->next;
	}
}

void	ft_expand(t_cmd *cmds, t_env *env, int exit_code)
{
	while (cmds)
	{
		expand_cmd(cmds, env, exit_code);
		cmds = cmds->next;
	}
}
