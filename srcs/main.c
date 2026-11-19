/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/11/19 12:29:05 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_shell *shell)
{
	if (!shell || !shell->cmds)
		return ;
	if (!shell->cmds->next && (!shell->cmds->args || !shell->cmds->args[0]))
	{
		shell->exit_code = run_redir_only(shell->cmds);
		return ;
	}
	if (!shell->cmds->next)
		shell->exit_code = exec_simple(shell->cmds, shell->envp, shell);
	else if (!shell->cmds->next->next)
		shell->exit_code = exec_2_pipe(shell->cmds, shell->cmds->next, shell);
	else
		shell->exit_code = exec_n_pipe(shell);
}

static void	process_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	tokens = ft_lexer(input);
	if (tokens && ft_syntax_check(tokens))
	{
		shell->cmds = ft_parser(tokens, shell->envp, shell->exit_code);
		ft_expand(shell->cmds, shell->envp, shell->exit_code);
		if (g_signal != SIGINT)
			run_cmd(shell);
		redir_close_heredocs(shell->cmds);
	}
	else
		shell->exit_code = 2;
	ft_gc_clear();
}

static void	run_shell(t_shell *shell)
{
	char	*input;

	set_prompt_signals();
	while (1)
	{
		input = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell->exit_code = 130;
			g_signal = 0;
		}
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		shell->input = input;
		process_input(shell, input);
		free(input);
		shell->input = NULL;
	}
}

int	main(char **envp)
{
	t_shell	shell;

	shell.envp = ft_env_init(envp);
	shell.exit_code = 0;
	shell.cmds = NULL;
	shell.input = NULL;
	shell.saved_in = -1;
	shell.saved_out = -1;
	get_shell(&shell);
	run_shell(&shell);
	clean_exit(shell.exit_code);
	return (0);
}
