/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 16:02:02 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/09 15:53:31 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "structs.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef HOSTNAME
#  define HOSTNAME "host"
# endif

// ====Env===
t_env	*ft_env_new(char *key, char *value);
void	ft_env_add_back(t_env **env, t_env *new_env_node);
t_env	*ft_env_from_line(char *line);
t_env	*ft_env_init(char **envp);
void ft_env_list(t_env **envp);

// ====Lexer===
t_token	*ft_token_new(t_token_type type, char *value, char quote);
void	ft_token_add_back(t_token **lst, t_token *new);
char	*ft_token_value_dup(const char *src);
int		ft_token_list_size(t_token *lst);
t_token	*ft_lex_word(const char *input, int *i);
t_token	*ft_lexer(const char *input);

//Utils
int		ft_isspace(char c);
int		ft_isoper(char c);
int		ft_iswordend(char c, char quote);
void	ft_print_tokens(t_token *tokens);
t_token	*print_err_syntax(void);

#endif