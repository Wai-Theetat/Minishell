/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:54:24 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:36:40 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_append_str(char *fst, char *snd, size_t sndlen)
{
	char	*result;
	size_t	len_fst;
	size_t	i;

	if (!fst || !snd)
		return (NULL);
	len_fst = ft_strlen(fst);
	result = malloc(len_fst + sndlen + 1);
	if (!result)
		return (free(fst), NULL);
	i = 0;
	while (i < len_fst)
	{
		result[i] = fst[i];
		i++;
	}
	while (snd && i < len_fst + sndlen)
	{
		result[i] = snd[i - len_fst];
		i++;
	}
	result[i] = '\0';
	free(fst);
	return (result);
}

char	**get_buffer_ref(int fd)
{
	static char	*buffers[OPEN_FD_SIZE];

	if (fd < 0 || fd >= OPEN_FD_SIZE)
		return (NULL);
	return (&buffers[fd]);
}

void	gnl_cleanup(int fd)
{
	char	**buf_ptr;

	buf_ptr = get_buffer_ref(fd);
	if (buf_ptr && *buf_ptr)
	{
		free(*buf_ptr);
		*buf_ptr = NULL;
	}
}
