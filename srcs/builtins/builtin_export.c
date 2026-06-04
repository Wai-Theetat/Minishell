/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +#+#+#+#+#+  
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 18:10:00 by copilot           #+#    #+#             */
/*   Updated: 2026/06/03 18:10:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_export_value(t_shell *shell, char *arg, char *eq)
{
	char	*key;

	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return ;
	ft_env_set(&shell->envp, key, eq + 1);
	free(key);
}

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int		index;
	char	*eq;

	if (!cmd || !shell || !cmd->args)
		return (1);
	index = 1;
	if (!cmd->args[index])
		return (0);
	while (cmd->args[index])
	{
		eq = ft_strchr(cmd->args[index], '=');
		if (eq)
			set_export_value(shell, cmd->args[index], eq);
		else
			ft_env_set(&shell->envp, cmd->args[index], "");
		index++;
	}
	return (0);
}
