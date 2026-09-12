/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	argv_init(t_argv *a)
{
	a->cap = 16;
	a->n = 0;
	a->v = ft_gc_calloc(a->cap + 1, sizeof(char *));
}

void	argv_push(t_argv *a, char *s)
{
	char	**nv;
	int		i;

	if (!a->v)
		return ;
	if (a->n + 1 >= a->cap)
	{
		nv = ft_gc_calloc(a->cap * 2 + 1, sizeof(char *));
		if (!nv)
			return ;
		i = -1;
		while (++i < a->n)
			nv[i] = a->v[i];
		a->v = nv;
		a->cap *= 2;
	}
	a->v[a->n++] = s;
	a->v[a->n] = NULL;
}

static void	field_copy(t_wstr *out, t_wstr *src, int start, int end)
{
	int	i;

	wstr_init(out);
	i = start;
	while (i < end)
	{
		wstr_addc(out, src->s[i], src->m[i]);
		i++;
	}
}

/*
** Splits on blanks that came from an unquoted expansion only: literal
** blanks are always protected because the lexer already ended the word on
** them.  Returns the number of fields written into out.
*/
int	split_fields(t_wstr *w, t_wstr *out, int max)
{
	int	i;
	int	start;
	int	n;

	i = 0;
	n = 0;
	while (i < w->len && n < max)
	{
		while (i < w->len && !w->m[i] && ft_isspace(w->s[i]))
			i++;
		if (i >= w->len)
			break ;
		start = i;
		while (i < w->len && !(!w->m[i] && ft_isspace(w->s[i])))
			i++;
		field_copy(&out[n++], w, start, i);
	}
	return (n);
}
