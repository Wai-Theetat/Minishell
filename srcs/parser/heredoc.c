/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 22:28:52 by tdharmar          #+#    #+#             */
/*   Updated: 2026/07/18 16:10:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_child(int pid, int *pipefd)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		g_signal = SIGINT;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		g_signal = SIGINT;
	if (g_signal == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

int	ft_heredoc(const char *delim, int expand, t_env *env, int exit_code)
{
	int		pipefd[2];
	int		pid;
	int		ret;
	t_hdoc	h;

	h.delim = delim;
	h.expand = expand;
	h.env = env;
	h.exit_code = exit_code;
	if (pipe(pipefd) == -1)
		return (-1);
	set_signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		set_prompt_signals();
		return (-1);
	}
	if (pid == 0)
		heredoc_child(pipefd, &h);
	ret = wait_child(pid, pipefd);
	set_prompt_signals();
	return (ret);
}

int	redir_run_heredocs(t_cmd *cmd, t_env *env, int exit_code)
{
	t_token	*node;

	node = cmd->redirs;
	while (node)
	{
		if (node->type == TOKEN_HEREDOC)
		{
			node->fd = ft_heredoc(node->value, node->quote == 0, env,
					exit_code);
			cmd->heredoc_fd = node->fd;
			if (node->fd == -1)
				return (-1);
		}
		node = node->next;
	}
	return (0);
}

void	redir_close_heredocs(t_cmd *cmds)
{
	t_token	*node;

	while (cmds)
	{
		node = cmds->redirs;
		while (node)
		{
			if (node->type == TOKEN_HEREDOC && node->fd != -1)
			{
				close(node->fd);
				node->fd = -1;
			}
			node = node->next;
		}
		cmds->heredoc_fd = -1;
		cmds = cmds->next;
	}
}
