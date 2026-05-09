/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:23:14 by koonchevych       #+#    #+#             */
/*   Updated: 2026/05/09 16:37:26 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_env *env)
{
	while (env)
	{
		ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

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

