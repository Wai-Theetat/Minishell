/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/31 02:11:31 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_env	t_env;
typedef struct s_cmd
{
	char			**args;
	char			*arg_quotes;
	char			*infile;
	char			*outfile;
	int				append;			// '>>' flag
	char			*heredoc;		// << "wait for *delimiter* to end"
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmds; // linked list of commands
	t_env	*envp; // linked list of environment path
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
	char			*value;	// NULL=op,str=WORD
	char			quote;
	struct s_token		*next;
}						t_token;

typedef struct s_buf
{
	char	*data;
	int		pos;
}	t_buf;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

#endif