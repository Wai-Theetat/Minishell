/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 12:59:05 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:33:06 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_isoper(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_iswordend(char c, char quote)
{
	if (quote)
		return (0);
	return (c == ' ' || c == '\t'
		|| c == '|' || c == '<' || c == '>');
}

int	is_ambiguous_redir(t_token *r)
{
	if (r->type == TOKEN_HEREDOC || r->quote != 0)
		return (0);
	if (!r->value || !r->value[0])
		return (1);
	if (ft_strchr(r->value, ' '))
		return (1);
	return (0);
}
