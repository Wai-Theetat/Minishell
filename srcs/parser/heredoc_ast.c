/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 22:28:52 by tdharmar          #+#    #+#             */
/*   Updated: 2026/07/18 16:10:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_run_heredocs(t_node *node, t_env *env, int exit_code)
{
	if (!node)
		return (0);
	if (node->type == NODE_CMD)
		return (redir_run_heredocs(node->cmd->redirs, env, exit_code));
	if (redir_run_heredocs(node->redirs, env, exit_code) == -1)
		return (-1);
	if (ast_run_heredocs(node->left, env, exit_code) == -1)
		return (-1);
	return (ast_run_heredocs(node->right, env, exit_code));
}

static void	close_list(t_token *redirs)
{
	while (redirs)
	{
		if (redirs->type == TOKEN_HEREDOC && redirs->fd != -1)
		{
			close(redirs->fd);
			redirs->fd = -1;
		}
		redirs = redirs->next;
	}
}

void	ast_close_heredocs(t_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		return (close_list(node->cmd->redirs));
	close_list(node->redirs);
	ast_close_heredocs(node->left);
	ast_close_heredocs(node->right);
}
