/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_line(t_shell *shell, t_token *tokens)
{
	shell->ast = ft_parser(tokens);
	if (!shell->ast)
	{
		shell->exit_code = 2;
		return ;
	}
	if (ast_run_heredocs(shell->ast, shell->envp, shell->exit_code) == -1)
		return ;
	if (g_signal != SIGINT)
		exec_node(shell->ast, shell);
	ast_close_heredocs(shell->ast);
}

static void	process_input(t_shell *shell, char *full)
{
	t_token	*tokens;

	tokens = ft_lexer(full);
	if (!tokens)
		shell->exit_code = 2;
	else if (tokens->type != TOKEN_EOF)
		run_line(shell, tokens);
	shell->ast = NULL;
	ft_gc_clear();
}

static void	run_shell(t_shell *shell)
{
	char	*input;
	char	*full;

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
	shell.ast = NULL;
	run_shell(&shell);
	return (shell.exit_code);
}
