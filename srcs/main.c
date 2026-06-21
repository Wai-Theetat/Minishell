/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/21 23:37:05 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_shell *shell)
{
	if (!shell || !shell->cmds || !shell->cmds->args || !shell->cmds->args[0])
		return ;
	if (!shell->cmds->next)
		shell->exit_code = exec_simple(shell->cmds, shell->envp, shell);
	else if (!shell->cmds->next->next)
		shell->exit_code = exec_2_pipe(shell->cmds, shell->cmds->next, shell);
	else
		shell->exit_code = exec_n_pipe(shell);
}

static void	process_input(t_shell *shell, char *full)
{
	t_token	*tokens;

	tokens = ft_lexer(full);
	if (tokens && ft_syntax_check(tokens))
	{
		shell->cmds = ft_parser(tokens, shell->envp, shell->exit_code);
		ft_expand(shell->cmds, shell->envp, shell->exit_code);
		if (g_signal != SIGINT)
			run_cmd(shell);
	}
	ft_gc_clear();
}

static void	run_shell(t_shell *shell)
{
	char	*input;
	char	*full;

	while (1)
	{
		g_signal = 0;
		input = readline("minishell$ ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		full = read_full_input(input);
		if (full != input)
			free(input);
		process_input(shell, full);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = ft_env_init(envp);
	shell.exit_code = 0;
	shell.cmds = NULL;
	run_shell(&shell);
	return (0);
}
