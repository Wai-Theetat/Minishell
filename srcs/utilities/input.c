/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 08:57:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/02 08:59:48 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_quote_state(const char *input)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote == '\'')
		return ("quote> ");
	if (quote == '"')
		return ("dquote> ");
	return (NULL);
}

static char	*get_pipe_state(const char *input)
{
	int	i;

	i = ft_strlen(input) - 1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return ("pipe> ");
	return (NULL);
}

char	*get_continue_prompt(const char *input)
{
	char	*prompt;

	prompt = get_quote_state(input);
	if (prompt)
		return (prompt);
	return (get_pipe_state(input));
}

static char	*join_lines(char *base, char *next)
{
	char	*with_newline;

	with_newline = ft_gc_strjoin(base, "\n");
	return (ft_gc_strjoin(with_newline, next));
}

char	*read_full_input(char *first)
{
	char	*input;
	char	*line;
	char	*prompt;

	input = first;
	prompt = get_continue_prompt(input);
	while (prompt)
	{
		line = readline(prompt);
		if (!line)
			return (input);
		input = join_lines(input, line);
		free(line);
		prompt = get_continue_prompt(input);
	}
	return (input);
}
