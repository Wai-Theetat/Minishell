/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:39:40 by tdharmar          #+#    #+#             */
/*   Updated: 2025/09/02 12:39:40 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_hs(unsigned int nb)
{
	unsigned long	n;

	n = nb;
	if (n > 15)
	{
		ft_putnbr_hs(n / 16);
		ft_putnbr_hs(n % 16);
	}
	else
		ft_putchar_fd("0123456789abcdef"[n], 1);
}

void	ft_putnbr_hb(unsigned int nb)
{
	unsigned long	n;

	n = nb;
	if (n > 15)
	{
		ft_putnbr_hb(n / 16);
		ft_putnbr_hb(n % 16);
	}
	else
		ft_putchar_fd("0123456789ABCDEF"[n], 1);
}

int	ft_print_hex(unsigned int nb, char format)
{
	int		cnt;
	long	n;

	n = nb;
	cnt = 0;
	if (n == 0)
		return (ft_print_char('0'));
	while (n != 0)
	{
		n /= 16;
		cnt++;
	}
	if (format == 'x')
		ft_putnbr_hs(nb);
	else if (format == 'X')
		ft_putnbr_hb(nb);
	return (cnt);
}
