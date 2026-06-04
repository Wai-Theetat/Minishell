/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:54:10 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/02 09:28:52 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_content(t_cmd *cmds)
{
	char	buf[1024];
	int		n;

	while (cmds)
	{
		if (cmds->heredoc_fd != -1)
		{
			ft_printf("  heredoc content:\n");
			n = read(cmds->heredoc_fd, buf, 1023);
			buf[n] = '\0';
			ft_printf("%s\n", buf);
			close(cmds->heredoc_fd);
			cmds->heredoc_fd = -1;
		}
		cmds = cmds->next;
	}
}

static void	process_input(t_shell *shell, char *full)
{
	t_token	*tokens;

	tokens = ft_lexer(full);
	if (tokens && ft_syntax_check(tokens))
	{
		shell->cmds = ft_parser(tokens);
		ft_expand(shell->cmds, shell->envp, shell->exit_code);
		ft_print_cmds(shell->cmds);
		print_heredoc_content(shell->cmds);
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
