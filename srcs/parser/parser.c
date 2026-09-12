/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*node_new(t_node_type type)
{
	t_node	*node;

	node = ft_gc_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	return (node);
}

static t_node	*join_nodes(t_node_type type, t_node *left, t_node *right)
{
	t_node	*node;

	node = node_new(type);
	if (!node)
		return (NULL);
	node->left = left;
	node->right = right;
	return (node);
}

t_node	*parse_pipeline(t_parse *p)
{
	t_node	*left;
	t_node	*right;

	left = parse_command(p);
	if (p->error)
		return (NULL);
	while (p->tok && p->tok->type == TOKEN_PIPE)
	{
		p->tok = p->tok->next;
		right = parse_command(p);
		if (p->error)
			return (NULL);
		left = join_nodes(NODE_PIPE, left, right);
	}
	return (left);
}

t_node	*parse_list(t_parse *p)
{
	t_node		*left;
	t_node		*right;
	t_node_type	type;

	left = parse_pipeline(p);
	if (p->error)
		return (NULL);
	while (p->tok && (p->tok->type == TOKEN_AND || p->tok->type == TOKEN_OR))
	{
		type = NODE_AND;
		if (p->tok->type == TOKEN_OR)
			type = NODE_OR;
		p->tok = p->tok->next;
		right = parse_pipeline(p);
		if (p->error)
			return (NULL);
		left = join_nodes(type, left, right);
	}
	return (left);
}

t_node	*ft_parser(t_token *tok)
{
	t_parse	p;
	t_node	*root;

	p.tok = tok;
	p.error = 0;
	root = parse_list(&p);
	if (p.error)
		return (NULL);
	if (p.tok && p.tok->type != TOKEN_EOF)
		return (syntax_error(&p, p.tok), NULL);
	return (root);
}
