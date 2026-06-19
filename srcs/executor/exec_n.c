/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_n.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:51:16 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/19 23:55:23 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_n_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pids[1024];
	size_t	i;
	t_cmd	*current_cmd;
	int		prev_read;
	int		status;

	i = 0;
	current_cmd = shell->cmds;
	prev_read = -1;
	while (current_cmd)
	{
		if (current_cmd->next)
			pipe(fd);
		pids[i] = fork();
		if (pids[i] == 0)
			cmd_loop(prev_read, fd, current_cmd, shell);
		if (prev_read != -1)
			close(prev_read);
		if (current_cmd->next)
		{
			close(fd[1]);
			prev_read = fd[0];
		}
		current_cmd = current_cmd->next;
		i++;
	}
	waitpid(pids[--i], &status, 0);
	while (i-- > 0)
		waitpid(pids[i], NULL, 0);
	shell->exit_code = pipe_status_code(status);
	return (shell->exit_code);
}

void	cmd_loop(int prev_read, int fd[2], t_cmd *current_cmd, t_shell *shell)
{
	if (prev_read != -1)
		dup2(prev_read, STDIN_FILENO);
	if (current_cmd->next)
		dup2(fd[1], STDOUT_FILENO);
	if (prev_read != -1)
		close(prev_read);
	if (current_cmd->next)
	{
		close(fd[0]);
		close(fd[1]);
	}
	exec_each_cmd(current_cmd, shell);
	exit(1);
}
