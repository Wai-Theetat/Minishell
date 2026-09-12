/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Only an unquoted '*' is a wildcard: pm[i] is non zero for the characters
** that were written between quotes.
*/
static int	glob_match(const char *p, const char *pm, const char *s)
{
	if (!*p)
		return (!*s);
	if (*p == '*' && !*pm)
	{
		if (glob_match(p + 1, pm + 1, s))
			return (1);
		if (*s)
			return (glob_match(p, pm, s + 1));
		return (0);
	}
	if (*s && *p == *s)
		return (glob_match(p + 1, pm + 1, s + 1));
	return (0);
}

int	has_wildcard(t_wstr *f)
{
	int	i;

	i = 0;
	while (i < f->len)
	{
		if (f->s[i] == '*' && !f->m[i])
			return (1);
		i++;
	}
	return (0);
}

static void	sort_matches(t_argv *a)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < a->n)
	{
		j = i + 1;
		while (j < a->n)
		{
			if (ft_collate_cmp(a->v[j], a->v[i]) < 0)
			{
				tmp = a->v[i];
				a->v[i] = a->v[j];
				a->v[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	scan_dir(t_argv *a, t_wstr *f, int base, char *dir)
{
	DIR				*dp;
	struct dirent	*ent;

	dp = opendir(dir);
	if (!dp)
		return ;
	ent = readdir(dp);
	while (ent)
	{
		if (!(ent->d_name[0] == '.' && f->s[base] != '.')
			&& glob_match(f->s + base, f->m + base, ent->d_name))
		{
			if (base == 0)
				argv_push(a, ft_gc_strdup(ent->d_name));
			else
				argv_push(a, ft_gc_strjoin(dir, ent->d_name));
		}
		ent = readdir(dp);
	}
	closedir(dp);
}

/*
** Expands one field.  Returns NULL when the field holds no wildcard or when
** nothing matched - bash then keeps the pattern as it is.
*/
char	**glob_word(t_wstr *f, int *count)
{
	t_argv	a;
	char	*dir;
	int		base;
	int		i;

	*count = 0;
	if (!has_wildcard(f))
		return (NULL);
	base = 0;
	i = 0;
	while (i < f->len)
		if (f->s[i++] == '/')
			base = i;
	dir = ft_gc_malloc(base + 2);
	ft_strlcpy(dir, f->s, base + 1);
	if (base == 0)
		ft_strlcpy(dir, ".", 2);
	argv_init(&a);
	scan_dir(&a, f, base, dir);
	if (a.n == 0)
		return (NULL);
	sort_matches(&a);
	*count = a.n;
	return (a.v);
}
