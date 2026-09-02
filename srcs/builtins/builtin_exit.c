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
#include <limits.h>

static int	is_overflow(const char *str, int sign)
{
	long long	limit_div;
	long long	limit_mod;
	long long	acc;
	int			digit;

	if (sign == 1)
	{
		limit_div = LLONG_MAX / 10;
		limit_mod = LLONG_MAX % 10;
	}
	else
	{
		limit_div = -(LLONG_MIN / 10);
		limit_mod = -(LLONG_MIN % 10);
	}
	acc = 0;
	while (*str)
	{
		digit = *str - '0';
		if (acc > limit_div || (acc == limit_div && digit > limit_mod))
			return (1);
		acc = acc * 10 + digit;
		str++;
	}
	return (0);
}

int	is_numeric(char *str)
{
	int	i;
	int	sign;

	i = 0;
	if (!str || !*str)
		return (0);
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (is_overflow(str + (str[0] == '+' || str[0] == '-'), sign))
		return (0);
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
