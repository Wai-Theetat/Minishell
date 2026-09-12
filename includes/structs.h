/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/06/22 09:54:33 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_cmd
{
	char				**args;
	char				*arg_quotes;
	t_token				*redirs;
	int					heredoc_fd;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_shell
{
	t_cmd				*cmds;
	t_env				*envp;
	int					exit_code;
}						t_shell;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	char				*raw;
	char				quote;
	int					ambiguous;
	int					fd;
	struct s_token		*next;
}						t_token;

typedef struct s_buf
{
	char				*data;
	int					pos;
}						t_buf;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_hdoc
{
	const char	*delim;
	int			expand;
	t_env		*env;
	int			exit_code;
}	t_hdoc;

#endif
