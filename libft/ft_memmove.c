/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:09:32 by tdharmar          #+#    #+#             */
/*   Updated: 2025/08/27 10:09:32 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*ptr_dest;
	const char	*ptr_src;

	if (!dest && !src)
		return (NULL);
	ptr_dest = dest;
	ptr_src = src;
	if (ptr_dest < ptr_src)
		while (n--)
			*ptr_dest++ = *ptr_src++;
	else
	{
		ptr_dest += n;
		ptr_src += n;
		while (n--)
			*(--ptr_dest) = *(--ptr_src);
	}
	return (dest);
}
