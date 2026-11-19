/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_second.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 14:46:06 by koonchevych       #+#    #+#             */
/*   Updated: 2026/06/19 23:35:47 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_each_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env;
	int		err;

	set_child_signals();
	if (apply_redirects(cmd) == -1)
		clean_exit(1);
	if (!cmd->args || !cmd->args[0])
		clean_exit(0);
	if (is_builtin(cmd->args[0]))
		clean_exit(run_builtin(cmd, shell));
	path = find_exec(cmd->args[0], shell->envp);
	if (!path)
		clean_exit(exec_not_found_code(cmd->args[0]));
	env = ft_env_to_char(shell->envp);
	execve(path, cmd->args, env);
	err = errno;
	perror(cmd->args[0]);
	free(path);
	ft_free_strs(env);
	if (err == ENOENT)
		clean_exit(127);
	clean_exit(126);
}

int	pipe_status_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
}

void	child_left(int fd[2], t_cmd *cmd, t_shell *shell)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	exec_each_cmd(cmd, shell);
}

static void	child_right(int fd[2], t_cmd *cmd, t_shell *shell)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	exec_each_cmd(cmd, shell);
}

int	exec_2_pipe(t_cmd *cmd_one, t_cmd *cmd_two, t_shell *shell)
{
	int		fd[2];
	int		status;
	pid_t	pid_1;
	pid_t	pid_2;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	set_exec_signals();
	pid_1 = fork();
	if (pid_1 == 0)
		child_left(fd, cmd_one, shell);
	pid_2 = fork();
	if (pid_2 == 0)
		child_right(fd, cmd_two, shell);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, &status, 0);
	set_prompt_signals();
	print_signal_msg(status);
	shell->exit_code = pipe_status_code(status);
	return (shell->exit_code);
}
