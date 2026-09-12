/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*token_text(t_token *tok)
{
	static const char	*names[] = {"", "|", "&&", "||", "(", ")", "<", ">",
		">>", "<<", "newline"};

	if (!tok)
		return (NULL);
	if (tok->type == TOKEN_WORD)
		return (tok->value);
	if (tok->type == TOKEN_EOF)
		return (NULL);
	return (names[tok->type]);
}

/*
** Reports the first token the parser could not accept.  A missing token
** (end of input) uses the same wording as bash so that the tester sees an
** identical trailing message.
*/
void	syntax_error(t_parse *p, t_token *tok)
{
	const char	*text;

	if (p->error)
		return ;
	p->error = 1;
	text = token_text(tok);
	if (!text)
	{
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
		return ;
	}
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)text, 2);
	ft_putstr_fd("'\n", 2);
}
