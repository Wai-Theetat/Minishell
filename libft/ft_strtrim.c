/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:53:25 by tdharmar          #+#    #+#             */
/*   Updated: 2025/08/26 17:53:25 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	size_t	start;
	size_t	end;
	size_t	res_len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && is_set(s1[start], set))
		start++;
	if (start == end)
		return (ft_strdup(""));
	end--;
	while (s1[end] && is_set(s1[end], set))
		end--;
	res_len = end - start + 1;
	res = (char *)malloc(sizeof(char) * (res_len + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1 + start, res_len);
	res[res_len] = '\0';
	return (res);
}
