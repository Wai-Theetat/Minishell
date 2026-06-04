/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:00:00 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/04 14:27:52 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != TOKEN_PIPE && tok->type != TOKEN_EOF)
	{
		if (tok->type == TOKEN_WORD)
			count++;
		tok = tok->next;
	}
	return (count);
}

static void	parse_redir(t_token **tok, t_cmd *cmd)
{
	t_token_type	type;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != TOKEN_WORD)
		return ;
	if (type == TOKEN_REDIRECT_IN)
		cmd->infile = (*tok)->value;
	else if (type == TOKEN_REDIRECT_OUT)
	{
		cmd->outfile = (*tok)->value;
		cmd->append = 0;
	}
	else if (type == TOKEN_REDIRECT_APPEND)
	{
		cmd->outfile = (*tok)->value;
		cmd->append = 1;
	}
	else if (type == TOKEN_HEREDOC)
		cmd->heredoc_delim = (*tok)->value;
	*tok = (*tok)->next;
}

static void	parse_word(t_token **tok, t_cmd *cmd, int *i)
{
	cmd->args[*i] = (*tok)->value;
	cmd->arg_quotes[(*i)++] = (*tok)->quote;
	*tok = (*tok)->next;
}

static t_cmd	*parse_cmd(t_token **tok)
{
	t_cmd	*cmd;
	int		argc;
	int		i;

	cmd = ft_gc_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->heredoc_fd = -1;
	argc = count_args(*tok);
	cmd->args = ft_gc_calloc(argc + 1, sizeof(char *));
	cmd->arg_quotes = ft_gc_calloc(argc + 1, sizeof(char));
	if (!cmd->args || !cmd->arg_quotes)
		return (NULL);
	i = 0;
	while (*tok && (*tok)->type != TOKEN_PIPE && (*tok)->type != TOKEN_EOF)
	{
		if ((*tok)->type == TOKEN_WORD)
			parse_word(tok, cmd, &i);
		else
			parse_redir(tok, cmd);
	}
	if (cmd->heredoc_delim)
		cmd->heredoc_fd = ft_heredoc(cmd->heredoc_delim);
	return (cmd);
}

t_cmd	*ft_parser(t_token *tok)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;

	head = NULL;
	last = NULL;
	while (tok && tok->type != TOKEN_EOF)
	{
		cmd = parse_cmd(&tok);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tok && tok->type == TOKEN_PIPE)
			tok = tok->next;
	}
	return (head);
}
