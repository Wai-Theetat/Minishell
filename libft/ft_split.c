/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:52:44 by tdharmar          #+#    #+#             */
/*   Updated: 2025/08/26 17:52:44 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_count(const char *str, char c)
{
	int		count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static size_t	ft_word_len(const char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

static char	*ft_word_dup(const char *str, size_t len)
{
	char	*word;
	size_t	i;

	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static void	*ft_free_all(char **arr, int size)
{
	while (size--)
		free(arr[size]);
	free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;

	i = 0;
	res = (char **)malloc(sizeof(char *) * (ft_word_count(s, c) + 1));
	if (!res || !s)
		return (NULL);
	while (*s)
	{
		while (*s && (*s == c))
			s++;
		if (*s && (*s != c))
		{
			res[i] = ft_word_dup(s, ft_word_len(s, c));
			if (!res[i])
				return (ft_free_all(res, i));
			i++;
			s += ft_word_len(s, c);
		}
	}
	res[i] = NULL;
	return (res);
}
