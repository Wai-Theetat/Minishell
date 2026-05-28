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

static int	check_pipe(t_token *tok)
{
	if (tok->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near token `|'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_token(t_token *tok)
{
	if (tok->type == TOKEN_PIPE)
	{
		if (!tok->next || tok->next->type == TOKEN_EOF
			|| tok->next->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error near token `|'\n", 2);
			return (0);
		}
	}
	if (is_redir(tok->type))
	{
		if (!tok->next || tok->next->type != TOKEN_WORD)
		{
			ft_putstr_fd("minishell: syntax error near token `newline'\n", 2);
			return (0);
		}
	}
	return (1);
}

int	ft_syntax_check(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return (1);
	if (!check_pipe(tok))
		return (0);
	while (tok && tok->type != TOKEN_EOF)
	{
		if (!check_token(tok))
			return (0);
		tok = tok->next;
	}
	return (1);
}
