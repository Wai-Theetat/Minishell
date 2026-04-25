/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:09:42 by tdharmar          #+#    #+#             */
/*   Updated: 2025/08/27 10:09:42 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	ch;

	ch = (unsigned char)c;
	while (*s)
	{
		if ((unsigned char)*s == ch)
			return ((char *)s);
		s++;
	}
	if ((unsigned char)*s == ch)
		return ((char *)s);
	return (NULL);
}
