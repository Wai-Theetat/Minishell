/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/21 22:32:00 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != TOKEN_PIPE && tok->type != TOKEN_EOF)
	{
		if (tok->type == TOKEN_WORD)
			count++;
		tok = tok->next;
	}
	return (count);
}

static void	parse_redir(t_token **tok, t_cmd *cmd)
{
	t_token_type	type;
	t_token			*node;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != TOKEN_WORD)
		return ;
	node = ft_token_new(type, (*tok)->value, (*tok)->quote);
	if (node)
	{
		node->raw = (*tok)->value;
		ft_token_add_back(&cmd->redirs, node);
	}
	*tok = (*tok)->next;
}

static void	parse_word(t_token **tok, t_cmd *cmd, int *i)
{
	cmd->args[*i] = (*tok)->value;
	cmd->arg_quotes[(*i)++] = (*tok)->quote;
	*tok = (*tok)->next;
}

static t_cmd	*parse_cmd(t_token **tok, t_env *env, int exit_code)
{
	t_cmd	*cmd;
	int		argc;
	int		i;

	cmd = ft_gc_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->heredoc_fd = -1;
	argc = count_args(*tok);
	cmd->args = ft_gc_calloc(argc + 1, sizeof(char *));
	cmd->arg_quotes = ft_gc_calloc(argc + 1, sizeof(char));
	if (!cmd->args || !cmd->arg_quotes)
		return (NULL);
	i = 0;
	while (*tok && (*tok)->type != TOKEN_PIPE && (*tok)->type != TOKEN_EOF)
	{
		if ((*tok)->type == TOKEN_WORD)
			parse_word(tok, cmd, &i);
		else
			parse_redir(tok, cmd);
	}
	if (redir_run_heredocs(cmd, env, exit_code) == -1)
		return (NULL);
	return (cmd);
}

t_cmd	*ft_parser(t_token *tok, t_env *env, int exit_code)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;

	head = NULL;
	last = NULL;
	while (tok && tok->type != TOKEN_EOF)
	{
		cmd = parse_cmd(&tok, env, exit_code);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tok && tok->type == TOKEN_PIPE)
			tok = tok->next;
	}
	return (head);
}
