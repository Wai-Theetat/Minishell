/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 02:25:15 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/01 13:17:58 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_gc_strjoin(const char *s1, const char *s2)
{
	size_t	len;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	res = ft_gc_malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, len + 1);
	ft_strlcat(res, s2, len + 1);
	return (res);
}

char	*ft_gc_itoa(int n)
{
	char	*str;
	char	*result;

	str = ft_itoa(n);
	if (!str)
		return (NULL);
	result = ft_gc_strjoin(str, "");
	free(str);
	return (result);
}
