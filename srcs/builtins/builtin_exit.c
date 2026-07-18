/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:21:59 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/03 17:58:04 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atol(char *str)
{
	long long	number;
	int			sign;
	int			i;

	number = 0;
	sign = 1;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + (str[i] - '0');
		i++;
	}
	return (number * sign);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	long long	n;

	if (!cmd->args[1])
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(shell->exit_code);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		write_msh_exec_arg_error("exit", cmd->args[1],
			"numeric argument required");
		exit(2);
	}
	if (cmd->args[2])
	{
		write_msh_exec_error("exit", "too many arguments");
		return (2);
	}
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	n = ft_atol(cmd->args[1]);
	exit((unsigned char)n);
}
