/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/03 13:37:19 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "constants.h"

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;			// '>>' flag
	char			*heredoc;		// << "wait for *delimiter* to end multi line string"
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmds;					// linked list of commands
	char	**envp;
	int		exit_code;
}	t_shell;

typedef struct s_token
{
	t_token_type	type;
	char			*value; 		// NULL for operators, actual text for WORD
	struct s_token	*next;
}	t_token;

#endif