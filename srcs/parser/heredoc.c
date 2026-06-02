/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/02 09:20:25 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	g_signal = sig;
	ft_putchar_fd('\n', 1);
	exit(1);
}

static void	write_lines(int fd, const char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_signal == SIGINT)
		{
			if (!line)
				ft_putstr_fd("minishell: warning: heredoc at EOF\n", 2);
			if (line)
				free(line);
			break ;
		}
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
	}
}

static void	run_child(int *pipefd, const char *delim)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	write_lines(pipefd[1], delim);
	close(pipefd[1]);
	exit(g_signal == SIGINT);
}

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

int	ft_heredoc(const char *delim)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		signal(SIGINT, SIG_DFL);
		return (-1);
	}
	if (pid == 0)
		run_child(pipefd, delim);
	return (wait_child(pid, pipefd));
}
