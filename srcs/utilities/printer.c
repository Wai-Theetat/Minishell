/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:17:18 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_token *tokens)
{
	const char	*type_names[] = {"WORD", "PIPE", "AND", "OR", "LPAREN",
		"RPAREN", "REDIRECT_IN", "REDIRECT_OUT", "REDIRECT_APPEND", "HEREDOC",
		"EOF"};

	while (tokens)
	{
		ft_printf("[%s]\t", type_names[tokens->type]);
		if (tokens->value)
			ft_printf("value='%s'\t", tokens->value);
		else
			ft_printf("(no value)\t");
		if (tokens->quote == '\'')
			ft_printf("quote=SINGLE\n");
		else if (tokens->quote == '"')
			ft_printf("quote=DOUBLE\n");
		else
			ft_printf("quote=NONE\n");
		tokens = tokens->next;
	}
}

t_token	*print_err_syntax(void)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching quote\n",
		2);
	return (NULL);
}

void	ft_print_cmds(t_cmd *cmds)
{
	int		i;
	t_token	*r;

	if (!cmds)
		return ;
	i = 0;
	while (cmds->args && cmds->args[i])
	{
		ft_printf("  args[%d] = '%s'\n", i, cmds->args[i]);
		i++;
	}
	r = cmds->redirs;
	while (r)
	{
		ft_printf("  redir type=%d value='%s'\n", r->type, r->value);
		r = r->next;
	}
}
