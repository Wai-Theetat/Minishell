/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/28 14:25:35 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND
		|| type == TOKEN_HEREDOC);
}

/* Operator tokens carry no value, so rebuild their text for messages. */
static char	*token_text(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return ("newline");
	if (tok->type == TOKEN_PIPE)
		return ("|");
	if (tok->type == TOKEN_REDIRECT_IN)
		return ("<");
	if (tok->type == TOKEN_REDIRECT_OUT)
		return (">");
	if (tok->type == TOKEN_REDIRECT_APPEND)
		return (">>");
	if (tok->type == TOKEN_HEREDOC)
		return ("<<");
	return (tok->value);
}

static int	syntax_error(t_token *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token_text(tok), 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}

static int	check_token(t_token *tok)
{
	if (tok->type == TOKEN_PIPE)
	{
		if (!tok->next || tok->next->type == TOKEN_EOF
			|| tok->next->type == TOKEN_PIPE)
			return (syntax_error(tok));
	}
	if (is_redir(tok->type))
	{
		if (!tok->next || tok->next->type != TOKEN_WORD)
			return (syntax_error(tok->next));
	}
	return (1);
}

int	ft_syntax_check(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return (1);
	if (tok->type == TOKEN_PIPE)
		return (syntax_error(tok));
	while (tok && tok->type != TOKEN_EOF)
	{
		if (!check_token(tok))
			return (0);
		tok = tok->next;
	}
	return (1);
}
