/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:39:45 by tdharmar          #+#    #+#             */
/*   Updated: 2025/09/02 12:39:45 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_put_ptr_hex(unsigned long nb)
{
	unsigned long long	n;

	n = nb;
	if (n > 15)
	{
		ft_put_ptr_hex(n / 16);
		ft_put_ptr_hex(n % 16);
	}
	else
		ft_putchar_fd("0123456789abcdef"[n], 1);
}

int	ft_print_ptr_hex(unsigned long nb)
{
	int					cnt;
	unsigned long long	n;

	n = nb;
	cnt = 0;
	if (n == 0)
		return (ft_print_char('0'));
	while (n != 0)
	{
		n /= 16;
		cnt++;
	}
	ft_put_ptr_hex(nb);
	return (cnt);
}

int	ft_print_pointer(void *val)
{
	int				cnt;
	unsigned long	byte;

	byte = (unsigned long) val;
	if (val == NULL)
		return (ft_print_str("(nil)"));
	cnt = ft_print_str("0x") + ft_print_ptr_hex(byte);
	return (cnt);
}
