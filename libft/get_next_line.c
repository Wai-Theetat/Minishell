/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/02 12:41:56 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_read_more(int fd)
{
	char	*result;
	char	*buf;
	int		bytes_read;

	result = ft_calloc(1, 1);
	buf = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!result || !buf)
		return (free(result), free(buf), NULL);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	if (bytes_read < 0)
		return (free(result), free(buf), NULL);
	while (bytes_read > 0)
	{
		buf[bytes_read] = '\0';
		result = ft_append_str(result, buf, bytes_read);
		if (!result)
			return (free(buf), NULL);
		if (ft_strchr(buf, '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(result), free(buf), NULL);
	}
	free(buf);
	return (result);
}

char	*ft_update_buf(int fd, char *txt)
{
	char	*updated;
	char	*result;

	if (!txt)
		return (ft_read_more(fd));
	if (txt[0] == '\0')
	{
		free(txt);
		return (ft_read_more(fd));
	}
	updated = ft_read_more(fd);
	if (!updated)
	{
		free(txt);
		return (NULL);
	}
	result = ft_append_str(txt, updated, ft_strlen(updated));
	if(!result)
	{
		free(updated);
		return (NULL);
	}
	free(updated);
	return (result);
}

char	*ft_get_first_line(char *txt)
{
	char	*line_result;
	size_t	len;

	if (!txt)
		return (ft_calloc(1, 1));
	len = 0;
	line_result = ft_calloc(1, 1);
	if (!line_result)
		return (NULL);
	while (txt[len])
	{
		if (txt[len] == '\n')
		{
			len++;
			break ;
		}
		len++;
	}
	line_result = ft_append_str(line_result, txt, len);
	return (line_result);
}

char	*ft_trim_first_line(char *txt)
{
	char	*result;
	size_t	i;
	size_t remaining_len;
	
	i = 0;
	while (txt[i] && txt[i] != '\n')
		i++;
	if (!txt[i])
		return (free(txt), NULL);
	remaining_len = ft_strlen(txt) - i;
	result = ft_calloc(remaining_len + 1, 1);
	if (!result)
	{
		free(txt);
		return (NULL);
	}
	ft_strlcat(result, &txt[i + 1], remaining_len + 1);
	free(txt);
	return (result);
}

char	*get_next_line(int fd)
{
	char		**buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = get_buffer_ref(fd);
	if (!buffer)
		return (NULL);
	*buffer = ft_update_buf(fd, *buffer);
	if (!*buffer)
		return (NULL);
	line = ft_get_first_line(*buffer);
	if (!line)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	*buffer = ft_trim_first_line(*buffer);
	if (line[0] == '\0')
		return (free(line), NULL);
	return (line);
}
