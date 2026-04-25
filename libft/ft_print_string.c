/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:39:49 by tdharmar          #+#    #+#             */
/*   Updated: 2025/09/02 12:39:49 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_str(char *str)
{
	int	cnt;

	cnt = 0;
	if (str == NULL)
		return (ft_print_str("(null)"));
	while (str[cnt])
	{
		ft_putchar_fd(str[cnt], 1);
		cnt++;
	}
	return (cnt);
}
