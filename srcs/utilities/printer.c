/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:17:18 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/04 15:47:14 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_token *tokens)
{
	const char	*type_names[] = {"WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
		"REDIRECT_APPEND", "HEREDOC", "EOF"};

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
	ft_printf("minishell: syntax error: unclosed quote\n");
	return (NULL);
}

void	ft_print_cmds(t_cmd *cmds)
{
	int	i;
	int	cmd_num;

	cmd_num = 0;
	while (cmds)
	{
		ft_printf("--- CMD %d ---\n", cmd_num++);
		i = 0;
		while (cmds->args && cmds->args[i])
		{
			ft_printf("  args[%d] = '%s'\n", i, cmds->args[i]);
			i++;
		}
		if (cmds->infile)
			ft_printf("  infile        = '%s'\n", cmds->infile);
		if (cmds->outfile)
			ft_printf("  outfile       = '%s' (append=%d)\n", cmds->outfile,
				cmds->append);
		if (cmds->heredoc_delim)
			ft_printf("  heredoc_delim = '%s'\n", cmds->heredoc_delim);
		if (cmds->heredoc_fd != -1)
			ft_printf("  heredoc_fd    = %d\n", cmds->heredoc_fd);
		cmds = cmds->next;
	}
}
