/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:17:18 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:49:43 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_token *tokens)
{
	const char	*type_names[] = {
		"WORD",
		"PIPE",
		"REDIRECT_IN",
		"REDIRECT_OUT",
		"REDIRECT_APPEND",
		"HEREDOC",
		"EOF"
	};

	while (tokens)
	{
		ft_printf("[%s] ", type_names[tokens->type]);
		if (tokens->value)
			ft_printf("value='%s'\n", tokens->value);
		else
			ft_printf("(no value)\n");
		tokens = tokens->next;
	}
}

t_token	*print_err_syntax(void)
{
	ft_printf("minishell: syntax error: unclosed quote\n");
	return (NULL);
}
