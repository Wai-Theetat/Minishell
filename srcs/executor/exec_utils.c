/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 14:25:52 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/15 17:13:07 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_infile(t_cmd *cmd)
{
	int	fd;

	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (0);
	}
	if (!cmd->infile)
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
		return (perror(cmd->infile), -1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	apply_outfile(t_cmd *cmd)
{
	int	fd;

	if (!cmd->outfile)
		return (0);
	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(cmd->outfile), -1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	apply_redirects(t_cmd *cmd)
{
	if (apply_infile(cmd) == -1)
		return (-1);
	if (apply_outfile(cmd) == -1)
		return (-1);
	return (0);
}
