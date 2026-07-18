/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +#+#+#+#+#+  
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 18:10:00 by copilot           #+#    #+#             */
/*   Updated: 2026/06/03 18:10:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_export_value(t_shell *shell, char *arg, char *eq)
{
	char	*key;

	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return ;
	ft_env_set(&shell->envp, key, eq + 1);
	free(key);
}

/* A name must start with a letter or '_', then letters, digits or '_'. */
static int	is_valid_identifier(char *arg)
{
	int	i;

	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_one(t_shell *shell, char *arg)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		set_export_value(shell, arg, eq);
	else
		ft_env_set(&shell->envp, arg, "");
}

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int	index;
	int	status;

	if (!cmd || !shell || !cmd->args)
		return (1);
	index = 1;
	status = 0;
	while (cmd->args[index])
	{
		if (!is_valid_identifier(cmd->args[index]))
		{
			write_msh_exec_arg_error("export", cmd->args[index],
				"not a valid identifier");
			status = 1;
		}
		else
			export_one(shell, cmd->args[index]);
		index++;
	}
	return (status);
}
