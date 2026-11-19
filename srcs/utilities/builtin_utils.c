/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 11:14:55 by koonchevych       #+#    #+#             */
/*   Updated: 2026/11/19 12:21:50 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strncmp(cmd_name, "cd", 3) == 0
		|| ft_strncmp(cmd_name, "echo", 5) == 0
		|| ft_strncmp(cmd_name, "pwd", 4) == 0
		|| ft_strncmp(cmd_name, "export", 7) == 0
		|| ft_strncmp(cmd_name, "unset", 6) == 0
		|| ft_strncmp(cmd_name, "env", 4) == 0
		|| ft_strncmp(cmd_name, "exit", 5) == 0);
}

int	run_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->args[0];
	if (ft_strncmp(name, "cd", 3) == 0)
		return (builtin_cd(cmd, shell));
	else if (ft_strncmp(name, "echo", 5) == 0)
		return (builtin_echo(cmd));
	else if (ft_strncmp(name, "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(name, "export", 7) == 0)
		return (builtin_export(cmd, shell));
	else if (ft_strncmp(name, "unset", 6) == 0)
		return (builtin_unset(cmd, shell));
	else if (ft_strncmp(name, "env", 4) == 0)
		return (builtin_env(shell->envp));
	else if (ft_strncmp(name, "exit", 5) == 0)
		return (builtin_exit(cmd, shell));
	return (1);
}

static void	restore_fds(t_shell *shell)
{
	if (shell->saved_in != -1)
	{
		dup2(shell->saved_in, STDIN_FILENO);
		close(shell->saved_in);
	}
	if (shell->saved_out != -1)
	{
		dup2(shell->saved_out, STDOUT_FILENO);
		close(shell->saved_out);
	}
	shell->saved_in = -1;
	shell->saved_out = -1;
}

int	run_builtin_redir(t_cmd *cmd, t_shell *shell)
{
	int	ret;

	shell->saved_in = dup(STDIN_FILENO);
	shell->saved_out = dup(STDOUT_FILENO);
	if (shell->saved_in == -1 || shell->saved_out == -1)
		return (restore_fds(shell), perror("dup"), 1);
	if (apply_redirects(cmd) == -1)
		return (restore_fds(shell), 1);
	ret = run_builtin(cmd, shell);
	restore_fds(shell);
	return (ret);
}
