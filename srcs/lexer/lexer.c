/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 13:38:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:48:57 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*lex_double_operator(const char *input, int *i)
{
	t_token	*token;

	if (input[*i] == '<')
		token = ft_token_new(TOKEN_HEREDOC, NULL);
	else
		token = ft_token_new(TOKEN_REDIRECT_APPEND, NULL);
	(*i) += 2;
	return (token);
}

static t_token	*lex_operator(const char *input, int *i)
{
	t_token	*token;

	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
		return (lex_double_operator(input, i));
	if (input[*i] == '|')
		token = ft_token_new(TOKEN_PIPE, NULL);
	else if (input[*i] == '<')
		token = ft_token_new(TOKEN_REDIRECT_IN, NULL);
	else
		token = ft_token_new(TOKEN_REDIRECT_OUT, NULL);
	(*i)++;
	return (token);
}

t_token	*ft_lexer(const char *input)
{
	t_token	*tokens;
	t_token	*token;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (ft_isoper(input[i]))
			token = lex_operator(input, &i);
		else
			token = ft_lex_word(input, &i);
		if (!token)
			return (print_err_syntax());
		ft_token_add_back(&tokens, token);
	}
	ft_token_add_back(&tokens, ft_token_new(TOKEN_EOF, NULL));
	return (tokens);
}
