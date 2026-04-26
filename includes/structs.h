/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 19:47:11 by tdharmar          #+#    #+#             */
/*   Updated: 2026/04/26 20:15:22 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;			// >> flag
	char			*heredoc;		// << wait for this to end multi line string
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmds;					// linked list of commands
	char	**envp;
	int		exit_code;
}	t_shell;

#endif