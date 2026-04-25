/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:40:04 by tdharmar          #+#    #+#             */
/*   Updated: 2025/09/02 12:40:04 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_from_format(char format, va_list args)
{
	if (format == 'c')
		return (ft_print_char(va_arg(args, int)));
	else if (format == 's')
		return (ft_print_str(va_arg(args, char *)));
	else if (format == 'p')
		return (ft_print_pointer(va_arg(args, void *)));
	else if ((format == 'd') || (format == 'i'))
		return (ft_print_dec(va_arg(args, int)));
	else if (format == 'u')
		return (ft_print_unsigned(va_arg(args, unsigned int)));
	else if ((format == 'x') || (format == 'X'))
		return (ft_print_hex(va_arg(args, unsigned int), format));
	else if (format == '%')
		return (ft_print_char('%'));
	else
		return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	size_t	i;
	int		print_cnt;

	va_start(args, str);
	print_cnt = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			print_cnt += ft_print_from_format(str[i], args);
		}
		else
		{
			ft_putchar_fd(str[i], 1);
			print_cnt ++;
		}
		i++;
	}
	va_end(args);
	return (print_cnt);
}
