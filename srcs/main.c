/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/28 14:26:45 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_shell(t_shell *shell)
{
	char	*input;
	t_token	*tokens;

	(void)shell;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = ft_lexer(input);
		if (tokens && ft_syntax_check(tokens))
		{
			// ft_print_tokens(tokens);
			shell->cmds = ft_parser(tokens);
			ft_print_cmds(shell->cmds);
		}
		ft_gc_clear();
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
