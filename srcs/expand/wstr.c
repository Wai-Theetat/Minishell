/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wstr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wstr_init(t_wstr *w)
{
	w->cap = 64;
	w->len = 0;
	w->s = ft_gc_malloc(w->cap + 1);
	w->m = ft_gc_calloc(w->cap + 1, 1);
	if (w->s)
		w->s[0] = '\0';
}

static void	wstr_grow(t_wstr *w, int extra)
{
	char	*s;
	char	*m;
	int		cap;
	int		i;

	if (w->len + extra < w->cap)
		return ;
	cap = w->cap * 2;
	while (cap <= w->len + extra)
		cap *= 2;
	s = ft_gc_malloc(cap + 1);
	m = ft_gc_calloc(cap + 1, 1);
	if (!s || !m)
		return ;
	i = -1;
	while (++i < w->len)
	{
		s[i] = w->s[i];
		m[i] = w->m[i];
	}
	s[w->len] = '\0';
	w->s = s;
	w->m = m;
	w->cap = cap;
}

void	wstr_addc(t_wstr *w, char c, char m)
{
	wstr_grow(w, 1);
	if (w->len + 1 > w->cap)
		return ;
	w->m[w->len] = m;
	w->s[w->len++] = c;
	w->s[w->len] = '\0';
}

void	wstr_addstr(t_wstr *w, const char *s, char m)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		wstr_addc(w, s[i++], m);
}

char	*ft_gc_strdup(const char *s)
{
	char	*dup;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = ft_gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}
