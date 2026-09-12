/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:28:29 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	word_len(const char *input, int i)
{
	char	quote;
	int		len;

	len = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i++];
		else if (quote && input[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (ft_iswordend(input, i, quote))
			break ;
		else
		{
			len++;
			i++;
		}
	}
	if (quote)
		return (-1);
	return (len);
}

static void	handle_quote(const char *input, int *pos, char *quote, char *q)
{
	if (*q == 0)
		*q = input[*pos];
	*quote = input[(*pos)++];
}

/*
** Every character kept in buf->data gets a mask byte:
**   0 = unquoted, 1 = inside double quotes, 2 = inside single quotes.
*/
static void	fill_buf(const char *input, int *pos, t_buf *buf, char *q)
{
	char	quote;

	quote = 0;
	while (input[*pos])
	{
		if (!quote && (input[*pos] == '\'' || input[*pos] == '"'))
			handle_quote(input, pos, &quote, q);
		else if (quote && input[*pos] == quote)
		{
			quote = 0;
			(*pos)++;
		}
		else if (ft_iswordend(input, *pos, quote))
			break ;
		else
		{
			buf->mask[buf->pos] = (quote == '"') + 2 * (quote == '\'');
			buf->data[(buf->pos)++] = input[(*pos)++];
		}
	}
}

static char	*collect_word(const char *input, int *pos, char *quote_out,
		char **mask_out)
{
	t_buf	buf;
	int		len;

	len = word_len(input, *pos);
	if (len == -1)
		return (NULL);
	buf.data = ft_gc_malloc(len + 1);
	buf.mask = ft_gc_calloc(len + 1, 1);
	if (!buf.data || !buf.mask)
		return (NULL);
	buf.pos = 0;
	*quote_out = 0;
	fill_buf(input, pos, &buf, quote_out);
	buf.data[buf.pos] = '\0';
	buf.mask[buf.pos] = 0;
	*mask_out = buf.mask;
	return (buf.data);
}

t_token	*ft_lex_word(const char *input, int *i)
{
	char	*value;
	char	*mask;
	char	quote;
	t_token	*token;

	value = collect_word(input, i, &quote, &mask);
	if (!value)
		return (NULL);
	token = ft_token_new(TOKEN_WORD, value, quote);
	if (token)
		token->mask = mask;
	return (token);
}
