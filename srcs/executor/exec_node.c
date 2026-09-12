/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_cmd_node(t_node *node, t_shell *shell)
{
	ft_expand_cmd(node->cmd, shell->envp, shell->exit_code);
	if (!node->cmd->args || !node->cmd->args[0])
	{
		shell->exit_code = run_redir_only(node->cmd);
		return (shell->exit_code);
	}
	shell->exit_code = exec_simple(node->cmd, shell->envp, shell);
	return (shell->exit_code);
}

static int	exec_logical(t_node *node, t_shell *shell)
{
	int	code;

	code = exec_node(node->left, shell);
	if (g_signal == SIGINT)
		return (code);
	if (node->type == NODE_AND && code != 0)
		return (code);
	if (node->type == NODE_OR && code == 0)
		return (code);
	return (exec_node(node->right, shell));
}

int	exec_node(t_node *node, t_shell *shell)
{
	if (!node)
		return (shell->exit_code);
	if (node->type == NODE_CMD)
		return (exec_cmd_node(node, shell));
	if (node->type == NODE_PIPE)
		return (exec_pipeline(node, shell));
	if (node->type == NODE_SUBSHELL)
		return (exec_subshell(node, shell));
	if (node->type == NODE_ARITH)
		return (exec_arith(node, shell));
	return (exec_logical(node, shell));
}

int	exec_subshell(t_node *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	set_exec_signals();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		set_child_signals();
		ft_expand_redirs(node->redirs, shell->envp, shell->exit_code);
		if (apply_redir_list(node->redirs) == -1)
			exit(1);
		exit(exec_node(node->left, shell));
	}
	while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
		;
	set_prompt_signals();
	print_signal_msg(status);
	shell->exit_code = pipe_status_code(status);
	return (shell->exit_code);
}

/*
** (( expression )) - only enough arithmetic to give the shell the same exit
** status bash does: 0 when the value is non zero, 1 when it is zero.
*/
int	exec_arith(t_node *node, t_shell *shell)
{
	t_token	*w;
	char	*val;
	long	result;

	result = 0;
	w = node->arith;
	while (w)
	{
		if (ft_isdigit(w->value[0]))
			result = ft_atoi(w->value);
		else
		{
			val = ft_env_get(shell->envp, w->value);
			result = 0;
			if (val)
				result = ft_atoi(val);
		}
		w = w->next;
	}
	shell->exit_code = (result == 0);
	return (shell->exit_code);
}
