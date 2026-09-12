/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ( list ) [redirections]
** A word may not follow the closing parenthesis, bash rejects it too.
*/
t_node	*parse_subshell(t_parse *p)
{
	t_node	*node;

	p->tok = p->tok->next;
	node = node_new(NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->left = parse_list(p);
	if (p->error)
		return (NULL);
	if (!p->tok || p->tok->type != TOKEN_RPAREN)
		return (syntax_error(p, p->tok), NULL);
	p->tok = p->tok->next;
	while (p->tok && is_redir_type(p->tok->type))
	{
		parse_redir(p, &node->redirs);
		if (p->error)
			return (NULL);
	}
	if (p->tok && p->tok->type == TOKEN_WORD)
		return (syntax_error(p, p->tok), NULL);
	return (node);
}

/*
** (( expression )) - the two opening parentheses are already known to be
** glued together.  Everything up to the matching pair is collected, the
** arithmetic value decides the exit status (0 -> 1, non zero -> 0).
*/
static int	arith_step(t_parse *p, t_node *node, int *depth)
{
	if (!p->tok || p->tok->type == TOKEN_EOF || p->tok->type == TOKEN_PIPE
		|| p->tok->type == TOKEN_AND || p->tok->type == TOKEN_OR)
		return (syntax_error(p, p->tok), 0);
	if (p->tok->type == TOKEN_LPAREN)
		(*depth)++;
	else if (p->tok->type == TOKEN_RPAREN)
		(*depth)--;
	else if (p->tok->type == TOKEN_WORD)
		ft_token_add_back(&node->arith, dup_word(p->tok, TOKEN_WORD));
	p->tok = p->tok->next;
	return (1);
}

t_node	*parse_command(t_parse *p)
{
	if (p->tok && p->tok->type == TOKEN_LPAREN)
	{
		if (p->tok->next && p->tok->next->type == TOKEN_LPAREN
			&& p->tok->next->glued)
			return (parse_arith(p));
		return (parse_subshell(p));
	}
	return (parse_simple(p));
}

t_node	*parse_arith(t_parse *p)
{
	t_node	*node;
	int		depth;

	node = node_new(NODE_ARITH);
	if (!node)
		return (NULL);
	p->tok = p->tok->next->next;
	depth = 2;
	while (depth > 0)
	{
		if (!arith_step(p, node, &depth))
			return (NULL);
	}
	return (node);
}
