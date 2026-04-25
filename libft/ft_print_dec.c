/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_dec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:39:36 by tdharmar          #+#    #+#             */
/*   Updated: 2025/09/02 12:39:36 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_dec(int nb)
{
	long	n;
	int		cnt;

	cnt = 0;
	n = nb;
	if (n == 0)
	{
		ft_putnbr_fd(nb, 1);
		return (1);
	}
	if (n < 0)
	{
		n *= -1;
		cnt++;
	}
	while (n != 0)
	{
		n /= 10;
		cnt++;
	}
	ft_putnbr_fd(nb, 1);
	return (cnt);
}
