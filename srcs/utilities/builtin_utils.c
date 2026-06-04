/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 11:14:55 by koonchevych       #+#    #+#             */
/*   Updated: 2026/05/20 11:15:21 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name, "echo") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0 || ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0 || ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0);
}

int	run_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->args[0];
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(cmd, shell));
	else if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd));
	else if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(name, "export") == 0)
		return (builtin_export(cmd, shell));
	else if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(cmd, shell));
	else if (ft_strcmp(name, "env") == 0)
		return (builtin_env(shell));
	else if (ft_strcmp(name, "exit") == 0)
		return (builtin_exit(cmd, shell));
	return (1);
}
