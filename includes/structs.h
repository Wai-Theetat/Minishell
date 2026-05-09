/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/05 13:12:37 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;			// '>>' flag
	char			*heredoc;		// << "*delimiter* to end str"
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmds;					// linked list of commands
	char	**envp;
	int		exit_code;
}	t_shell;

typedef enum e_token_type
{
	TOKEN_WORD,				/* Regular txt/arg */
	TOKEN_PIPE,				/* | */
	TOKEN_REDIRECT_IN,		/* < */
	TOKEN_REDIRECT_OUT,		/* > */
	TOKEN_REDIRECT_APPEND,	/* >> */
	TOKEN_HEREDOC,			/* << */
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;			// NULL=op,str=WORD
	struct s_token	*next;
}	t_token;

#endif