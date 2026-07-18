/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 16:10:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/07/18 16:10:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	g_signal = sig;
	ft_putchar_fd('\n', 1);
	exit(1);
}

static void	write_one_line(int fd, char *line, t_hdoc *h)
{
	char	*out;

	if (h->expand)
		out = expand_str(line, h->env, h->exit_code);
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
		write_one_line(fd, line, h);
		free(line);
	}
}

void	heredoc_child(int *pipefd, t_hdoc *h)
{
	set_signal(SIGINT, heredoc_sigint);
	set_signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	write_lines(pipefd[1], h);
	close(pipefd[1]);
	exit(g_signal == SIGINT);
}
