/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 15:40:00 by koonchevych       #+#    #+#             */
/*   Updated: 2026/07/18 15:40:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parent while a child runs: ignore both, only the child may die. */
void	set_exec_signals(void)
{
	set_signal(SIGINT, SIG_IGN);
	set_signal(SIGQUIT, SIG_IGN);
}

/* Child before execve: SIG_IGN survives execve, SIG_DFL must be restored. */
void	set_child_signals(void)
{
	set_signal(SIGINT, SIG_DFL);
	set_signal(SIGQUIT, SIG_DFL);
}

/* Bash prints a newline for SIGINT and "Quit: 3" for SIGQUIT. */
void	print_signal_msg(int status)
{
	if (!WIFSIGNALED(status))
		return ;
	if (WTERMSIG(status) == SIGINT)
		write(2, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(2, "Quit: 3\n", 8);
}
