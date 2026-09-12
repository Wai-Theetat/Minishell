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

static int	open_redir(t_token *redir)
{
	if (redir->type == TOKEN_REDIRECT_IN)
		return (open(redir->value, O_RDONLY));
	if (redir->type == TOKEN_REDIRECT_APPEND)
		return (open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644));
}

static int	apply_heredoc(t_cmd *cmd, t_token *redir)
{
	if (redir->fd == -1)
		return (-1);
	dup2(redir->fd, STDIN_FILENO);
	close(redir->fd);
	redir->fd = -1;
	cmd->heredoc_fd = -1;
	return (0);
}

static int	apply_one(t_cmd *cmd, t_token *redir)
{
	int	fd;

	if (redir->type == TOKEN_HEREDOC)
		return (apply_heredoc(cmd, redir));
	if (redir->ambiguous)
		return (write_msh_exec_error(redir->raw, "ambiguous redirect"), -1);
	fd = open_redir(redir);
	if (fd == -1)
		return (perror(redir->value), -1);
	if (redir->type == TOKEN_REDIRECT_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	apply_redirects(t_cmd *cmd)
{
	t_token	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (apply_one(cmd, redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

int	run_redir_only(t_cmd *cmd)
{
	int	saved_in;
	int	saved_out;
	int	ret;

	if (!cmd->redirs)
		return (0);
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
		return (perror("dup"), 1);
	ret = 0;
	if (apply_redirects(cmd) == -1)
		ret = 1;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (ret);
}
