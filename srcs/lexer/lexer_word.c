/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:28:29 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/09 13:55:40 by tdharmar         ###   ########.fr       */
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
		else if (ft_iswordend(input[i], quote))
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
	else if (*q != input[*pos])
		*q = 0;
	*quote = input[(*pos)++];
}

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
		else if (ft_iswordend(input[*pos], quote))
			break ;
		else
			buf->data[(buf->pos)++] = input[(*pos)++];
	}
}

static char	*collect_word(const char *input, int *pos, char *quote_out)
{
	t_buf	buf;
	int		len;

	len = word_len(input, *pos);
	if (len == -1)
		return (NULL);
	buf.data = ft_gc_malloc(len + 1);
	if (!buf.data)
		return (NULL);
	buf.pos = 0;
	*quote_out = 0;
	fill_buf(input, pos, &buf, quote_out);
	buf.data[buf.pos] = '\0';
	return (buf.data);
}

t_token	*ft_lex_word(const char *input, int *i)
{
	char	*value;
	char	quote;

	value = collect_word(input, i, &quote);
	if (!value)
		return (NULL);
	return (ft_token_new(TOKEN_WORD, value, quote));
}
