/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_type(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC);
}

t_token	*dup_word(t_token *src, t_token_type type)
{
	t_token	*node;

	node = ft_token_new(type, src->value, src->quote);
	if (!node)
		return (NULL);
	node->mask = src->mask;
	node->raw = src->value;
	return (node);
}

void	parse_redir(t_parse *p, t_token **redirs)
{
	t_token_type	type;
	t_token			*node;

	type = p->tok->type;
	p->tok = p->tok->next;
	if (!p->tok || p->tok->type != TOKEN_WORD)
	{
		syntax_error(p, p->tok);
		return ;
	}
	node = dup_word(p->tok, type);
	ft_token_add_back(redirs, node);
	p->tok = p->tok->next;
}

static void	simple_step(t_parse *p, t_cmd *cmd)
{
	if (p->tok->type == TOKEN_WORD)
	{
		ft_token_add_back(&cmd->words, dup_word(p->tok, TOKEN_WORD));
		p->tok = p->tok->next;
	}
	else
		parse_redir(p, &cmd->redirs);
}

t_node	*parse_simple(t_parse *p)
{
	t_cmd	*cmd;
	t_node	*node;

	cmd = ft_gc_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	while (p->tok && (p->tok->type == TOKEN_WORD
			|| is_redir_type(p->tok->type)))
	{
		simple_step(p, cmd);
		if (p->error)
			return (NULL);
	}
	if ((!cmd->words && !cmd->redirs) || (p->tok
			&& p->tok->type == TOKEN_LPAREN))
		return (syntax_error(p, p->tok), NULL);
	node = node_new(NODE_CMD);
	node->cmd = cmd;
	return (node);
}
