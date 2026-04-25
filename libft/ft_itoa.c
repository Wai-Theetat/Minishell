/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:51:20 by tdharmar          #+#    #+#             */
/*   Updated: 2025/08/26 17:51:20 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_len(long n)
{
	int	i;

	i = 0;
	if (n <= 0)
	{
		n *= -1;
		i++;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*ft_think(char *res, long nb, int i)
{
	if (nb == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (nb < 0)
	{
		res[0] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		res[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	int		i;

	nb = n;
	i = ft_count_len(nb);
	res = (char *)malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	res[i--] = '\0';
	return (ft_think(res, nb, i));
}
