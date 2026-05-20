/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:21:59 by koonchevych       #+#    #+#             */
/*   Updated: 2026/05/19 13:22:08 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	buitin_pwd(void)
{
	char	buffer_path[1024];

	if (!getcwd(buffer_path, 1024))
	{
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", buffer_path);
	return (0);
}

