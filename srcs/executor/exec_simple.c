/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:49:46 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/03 18:16:33 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_shell_exit_code(t_shell *shell, int code)
{
	if (shell)
		shell->exit_code = code;
	return (code);
}

static int	wait_for_child(pid_t pid, t_shell *shell)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
		{
			perror("waitpid");
			return (update_shell_exit_code(shell, -1));
		}
	}
	if (WIFEXITED(status))
		return (update_shell_exit_code(shell, WEXITSTATUS(status)));
	if (WIFSIGNALED(status))
		return (update_shell_exit_code(shell, 128 + WTERMSIG(status)));
	return (update_shell_exit_code(shell, -1));
}

static void	run_child(char *path, t_cmd *cmd, char **envp)
{
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	free(path);
	exit(126);
}

int	exec_simple(t_cmd *cmd, t_env *envp, t_shell *shell)
{
	char	*path;
	pid_t	pid;
	char	**exec_env;

	path = find_exec(cmd->args[0], envp);
	if (!path)
		return (127);
	pid = fork();
	if (pid < 0)
	{
		free(path);
		return (-1);
	}
	if (pid == 0)
	{
		exec_env = ft_env_to_char(envp);
		run_child(path, cmd, exec_env);
	}
	free(path);
	return (wait_for_child(pid, shell));
}
