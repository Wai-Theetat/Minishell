/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:28:29 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:46:41 by tdharmar         ###   ########.fr       */
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

static void	fill_buf(const char *input, int *i, char *buf, int *j)
{
	char	quote;

	quote = 0;
	while (input[*i])
	{
		if (!quote && (input[*i] == '\'' || input[*i] == '"'))
			quote = input[(*i)++];
		else if (quote && input[*i] == quote)
			quote = 0 * (*i)++;
		else if (ft_iswordend(input[*i], quote))
			break ;
		else
			buf[(*j)++] = input[(*i)++];
	}
}

static char	*collect_word(const char *input, int *i)
{
	char	*buf;
	int		len;
	int		j;

	len = word_len(input, *i);
	if (len == -1)
		return (NULL);
	buf = ft_gc_malloc(len + 1);
	if (!buf)
		return (NULL);
	j = 0;
	fill_buf(input, i, buf, &j);
	buf[j] = '\0';
	return (buf);
}

t_token	*ft_lex_word(const char *input, int *i)
{
	char	*value;

	value = collect_word(input, i);
	if (!value)
		return (NULL);
	return (ft_token_new(TOKEN_WORD, value));
}
