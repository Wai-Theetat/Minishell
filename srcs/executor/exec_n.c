/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_n.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:51:16 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	flatten_pipe(t_node *node, t_node **out, int i)
{
	if (node->type == NODE_PIPE)
	{
		i = flatten_pipe(node->left, out, i);
		return (flatten_pipe(node->right, out, i));
	}
	out[i++] = node;
	return (i);
}

static void	pipe_child(int prev, int *fd, t_node *node, t_shell *shell)
{
	if (prev != -1)
	{
		dup2(prev, STDIN_FILENO);
		close(prev);
	}
	if (fd[1] != -1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	child_exec_node(node, shell);
	exit(1);
}

static void	spawn_pipeline(t_node **items, int n, t_shell *shell, pid_t *pids)
{
	int	fd[2];
	int	prev;
	int	i;

	prev = -1;
	i = 0;
	while (i < n)
	{
		fd[0] = -1;
		fd[1] = -1;
		if (i + 1 < n && pipe(fd) == -1)
			return ;
		pids[i] = fork();
		if (pids[i] == 0)
			pipe_child(prev, fd, items[i], shell);
		if (prev != -1)
			close(prev);
		if (fd[1] != -1)
		{
			close(fd[1]);
			prev = fd[0];
		}
		i++;
	}
}

int	wait_last(pid_t last, pid_t *pids, int count)
{
	int	status;
	int	last_status;
	int	i;

	last_status = 0;
	i = 0;
	while (i < count)
	{
		status = 0;
		while (waitpid(pids[i], &status, 0) == -1 && errno == EINTR)
			;
		if (pids[i] == last)
			last_status = status;
		i++;
	}
	print_signal_msg(last_status);
	return (pipe_status_code(last_status));
}

int	exec_pipeline(t_node *node, t_shell *shell)
{
	t_node	*items[512];
	pid_t	pids[512];
	int		n;

	n = flatten_pipe(node, items, 0);
	set_exec_signals();
	spawn_pipeline(items, n, shell, pids);
	shell->exit_code = wait_last(pids[n - 1], pids, n);
	set_prompt_signals();
	return (shell->exit_code);
}
