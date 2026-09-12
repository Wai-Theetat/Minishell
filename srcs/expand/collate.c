/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 12:00:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Wildcard results have to come out in the same order bash prints them.
** bash sorts with the locale collation table, which is *not* byte order:
** punctuation follows its own sequence and sorts before digits and letters.
** The table below reproduces the primary weights of the common
** en_US.UTF-8 / iso14651_t1 order for printable ASCII.
*/
static int	collate_weight(unsigned char c)
{
	static const char	*order = " _-,;:!?.'\"()[]{}@*/\\&#%`^+<=>|~$"
		"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";
	char				*pos;

	if (!c)
		return (-1);
	pos = ft_strchr(order, c);
	if (pos && *pos)
		return ((int)(pos - order));
	return (256 + c);
}

int	ft_collate_cmp(const char *a, const char *b)
{
	int	wa;
	int	wb;

	while (*a && *b)
	{
		wa = collate_weight((unsigned char)*a);
		wb = collate_weight((unsigned char)*b);
		if (wa != wb)
			return (wa - wb);
		a++;
		b++;
	}
	return ((unsigned char)*a - (unsigned char)*b);
}
