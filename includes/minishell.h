/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 16:02:02 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/03 13:54:41 by tdharmar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "structs.h"
# include "constants.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef HOSTNAME
#  define HOSTNAME "host"
# endif

// ====Lexer===
t_token	*ft_token_new(t_token_type type, char *value);
void	ft_token_add_back(t_token **lst, t_token *new);
char	*ft_token_value_dup(const char *src);
int		ft_token_list_size(t_token *lst);
void	ft_print_tokens(t_token *tokens);
t_token	*ft_lexer(const char *input);


#endif