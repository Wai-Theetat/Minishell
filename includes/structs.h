/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_env	t_env;
typedef struct s_token	t_token;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}						t_token_type;

/*
** value : text with the quote characters already removed
** mask  : one byte per char of value, 1 when the char was inside quotes
**         (so it must not be word-split nor treated as a wildcard)
** quote : 0 when the word carried no quote at all
** glued : 1 when no blank separated this token from the previous one
*/
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	char				*mask;
	char				*raw;
	char				quote;
	int					glued;
	int					ambiguous;
	int					fd;
	struct s_token		*next;
}						t_token;

typedef struct s_cmd
{
	t_token				*words;
	char				**args;
	t_token				*redirs;
}						t_cmd;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
	NODE_ARITH
}						t_node_type;

/*
** left / right : operands of PIPE, AND, OR - left alone for SUBSHELL
** cmd          : simple command (NODE_CMD only)
** redirs       : redirections attached to a subshell
** arith        : word list of an arithmetic command  (( ... ))
*/
typedef struct s_node
{
	t_node_type			type;
	struct s_node		*left;
	struct s_node		*right;
	t_cmd				*cmd;
	t_token				*redirs;
	t_token				*arith;
}						t_node;

typedef struct s_shell
{
	t_node				*ast;
	t_env				*envp;
	int					exit_code;
}						t_shell;

typedef struct s_buf
{
	char				*data;
	char				*mask;
	int					pos;
}						t_buf;

/*
** A string that carries its protection mask along: m[i] is 1 when s[i] came
** from a quoted section and must not be word-split nor globbed.
*/
typedef struct s_wstr
{
	char				*s;
	char				*m;
	int					len;
	int					cap;
}						t_wstr;

typedef struct s_argv
{
	char				**v;
	int					n;
	int					cap;
}						t_argv;

typedef struct s_parse
{
	t_token				*tok;
	int					error;
}						t_parse;

typedef struct s_env
{
	char				*key;
	char				*value;
	int					has_value;
	struct s_env		*next;
}						t_env;

typedef struct s_hdoc
{
	const char			*delim;
	int					expand;
	t_env				*env;
	int					exit_code;
}						t_hdoc;

#endif
