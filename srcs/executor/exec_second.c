/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 14:46:06 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_each_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env;

	ft_expand_cmd(cmd, shell->envp, shell->exit_code);
	if (apply_redir_list(cmd->redirs) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, shell));
	path = find_exec(cmd->args[0], shell->envp);
	if (!path)
		exit(exec_not_found_code(cmd->args[0]));
	env = ft_env_to_char(shell->envp);
	execve(path, cmd->args, env);
	if (errno == ENOENT)
		exit((perror(cmd->args[0]), 127));
	exit((perror(cmd->args[0]), 126));
}

int	pipe_status_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
}

/*
** Runs one member of a pipeline (or a forked subshell) - never returns.
*/
void	child_exec_node(t_node *node, t_shell *shell)
{
	set_child_signals();
	if (node->type == NODE_CMD)
		exec_each_cmd(node->cmd, shell);
	if (node->type == NODE_SUBSHELL)
	{
		ft_expand_redirs(node->redirs, shell->envp, shell->exit_code);
		if (apply_redir_list(node->redirs) == -1)
			exit(1);
		exit(exec_node(node->left, shell));
	}
	exit(exec_node(node, shell));
}
