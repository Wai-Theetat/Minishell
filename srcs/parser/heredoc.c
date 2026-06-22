/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 22:28:52 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/21 22:29:57 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	g_signal = sig;
	ft_putchar_fd('\n', 1);
	exit(1);
}

static void	write_one_line(int fd, char *line, int expand,
				t_env *env, int exit_code)
{
	char	*out;

	if (expand)
		out = expand_str(line, env, exit_code);
	else
		out = line;
	ft_putstr_fd(out, fd);
	ft_putchar_fd('\n', fd);
}

static void	write_lines(int fd, t_hdoc *h)
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
		if (ft_strncmp(line, h->delim, ft_strlen(h->delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write_one_line(fd, line, h->expand, h->env, h->exit_code);
		free(line);
	}
}

static void	run_child(int *pipefd, t_hdoc *h)
{
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	write_lines(pipefd[1], h);
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

int	ft_heredoc(const char *delim, int expand, t_env *env, int exit_code)
{
	int		pipefd[2];
	int		pid;
	t_hdoc	h;

	h.delim = delim;
	h.expand = expand;
	h.env = env;
	h.exit_code = exit_code;
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
		run_child(pipefd, &h);
	return (wait_child(pid, pipefd));
}

