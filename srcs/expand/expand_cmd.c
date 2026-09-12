/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	push_field(t_argv *a, t_wstr *field)
{
	char	**matches;
	int		count;
	int		i;

	matches = glob_word(field, &count);
	if (!matches)
		return (argv_push(a, field->s));
	i = 0;
	while (i < count)
		argv_push(a, matches[i++]);
}

static void	push_word(t_argv *a, t_token *tok, t_env *env, int exit_code)
{
	t_wstr	w;
	t_wstr	*fields;
	int		n;
	int		i;

	w = expand_word(tok->value, tok->mask, env, exit_code);
	fields = ft_gc_calloc(w.len + 2, sizeof(t_wstr));
	if (!fields)
		return ;
	n = split_fields(&w, fields, w.len + 1);
	if (n == 0)
	{
		if (tok->quote)
			argv_push(a, ft_gc_strdup(""));
		return ;
	}
	i = 0;
	while (i < n)
		push_field(a, &fields[i++]);
}

/*
** A redirection target must expand to exactly one word, otherwise bash
** refuses it with "ambiguous redirect".
*/
static void	expand_redir_one(t_token *r, t_env *env, int exit_code)
{
	t_wstr	w;
	t_wstr	*fields;
	char	**matches;
	int		count;

	w = expand_word(r->value, r->mask, env, exit_code);
	fields = ft_gc_calloc(w.len + 2, sizeof(t_wstr));
	if (!fields)
		return ;
	r->ambiguous = 0;
	if (split_fields(&w, fields, w.len + 1) != 1)
	{
		r->ambiguous = (r->quote == 0);
		r->value = "";
		return ;
	}
	matches = glob_word(&fields[0], &count);
	if (matches && count > 1)
		return ((void)(r->ambiguous = 1));
	if (matches)
		r->value = matches[0];
	else
		r->value = fields[0].s;
}

void	ft_expand_redirs(t_token *redirs, t_env *env, int exit_code)
{
	while (redirs)
	{
		if (redirs->type != TOKEN_HEREDOC)
			expand_redir_one(redirs, env, exit_code);
		redirs = redirs->next;
	}
}

void	ft_expand_cmd(t_cmd *cmd, t_env *env, int exit_code)
{
	t_argv	a;
	t_token	*w;

	if (!cmd || cmd->args)
		return ;
	argv_init(&a);
	w = cmd->words;
	while (w)
	{
		push_word(&a, w, env, exit_code);
		w = w->next;
	}
	cmd->args = a.v;
	ft_expand_redirs(cmd->redirs, env, exit_code);
}
