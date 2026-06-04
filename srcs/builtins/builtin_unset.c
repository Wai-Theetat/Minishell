/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +#+#+#+#+#+  
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 18:10:00 by copilot           #+#    #+#             */
/*   Updated: 2026/06/03 18:10:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	index;

	if (!cmd || !shell || !cmd->args)
		return (1);
	index = 1;
	while (cmd->args[index])
	{
		ft_env_unset(&shell->envp, cmd->args[index]);
		index++;
	}
	return (0);
}
