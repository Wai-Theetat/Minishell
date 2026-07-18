/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitlin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:21:59 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/22 09:44:57 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_cd_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = ft_env_get(shell->envp, "HOME");
		if (!path)
		{
			write_msh_exec_error("cd", "HOME not set");
			return (NULL);
		}
	}
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
	{
		path = ft_env_get(shell->envp, "OLDPWD");
		if (!path)
		{
			write_msh_exec_error("cd", "OLDPWD not set");
			return (NULL);
		}
		ft_printf("%s\n", path);
	}
	else
		path = cmd->args[1];
	return (path);
}

static int	update_pwd(t_shell *shell, char *old_pwd)
{
	char	cwd[4096];

	if (old_pwd)
		ft_env_set(&shell->envp, "OLDPWD", old_pwd);
	if (!getcwd(cwd, sizeof(cwd)))
	{
		write_msh_exec_error("cd", strerror(errno));
		return (1);
	}
	ft_env_set(&shell->envp, "PWD", cwd);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	if (cmd->args[1] && cmd->args[2])
	{
		write_msh_exec_error("cd", "too many arguments");
		return (2);
	}
	path = ft_cd_path(cmd, shell);
	if (!path)
		return (1);
	old_pwd = ft_env_get(shell->envp, "PWD");
	if (chdir(path) != 0)
	{
		write_msh_exec_arg_error("cd", path, strerror(errno));
		return (1);
	}
	return (update_pwd(shell, old_pwd));
}
