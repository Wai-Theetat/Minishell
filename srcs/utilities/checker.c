/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 12:59:05 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

/*
** A lone '&' is kept as an ordinary character: only "&&" is an operator.
*/
int	ft_isoper(const char *s, int i)
{
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (1);
	if (s[i] == '(' || s[i] == ')')
		return (1);
	if (s[i] == '&' && s[i + 1] == '&')
		return (1);
	return (0);
}

int	ft_iswordend(const char *s, int i, char quote)
{
	if (quote)
		return (0);
	if (ft_isspace(s[i]))
		return (1);
	return (ft_isoper(s, i));
}

int	shell_interactive(void)
{
	return (isatty(STDIN_FILENO));
}
