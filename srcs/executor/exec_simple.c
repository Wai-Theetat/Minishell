/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:49:46 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/15 17:18:31 by koonchevych      ###   ########.fr       */
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
	if (apply_redirects(cmd) == -1)
	{
		free(path);
		exit(1);
	}
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	if (errno == ENOENT)
		return (free(path), exit(127));
	free(path);
	exit(126);
}

int	exec_external_cmd(t_cmd *cmd, t_env *envp, t_shell *shell)
{
	char	*path;
	pid_t	pid;
	char	**exec_env;

	path = find_exec(cmd->args[0], envp);
	if (!path)
		return (update_shell_exit_code(shell,
				exec_not_found_code(cmd->args[0])));
	pid = fork();
	if (pid < 0)
	{
		free(path);
		return (update_shell_exit_code(shell, -1));
	}
	if (pid == 0)
	{
		exec_env = ft_env_to_char(envp);
		run_child(path, cmd, exec_env);
	}
	free(path);
	return (wait_for_child(pid, shell));
}

int	exec_simple(t_cmd *cmd, t_env *envp, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (update_shell_exit_code(shell, 0));
	if (is_builtin(cmd->args[0]))
		return (run_builtin_redir(cmd, shell));
	return (exec_external_cmd(cmd, envp, shell));
}
