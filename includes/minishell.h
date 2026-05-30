/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdharmar <tdharmar@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 16:02:02 by tdharmar          #+#    #+#             */
/*   Updated: 2026/05/31 02:29:57 by tdharmar         ###   ########.fr       */
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
void	ft_env_list(t_env **envp); //just fortest
char	**ft_env_to_char(t_env *env);
char	*ft_env_get(t_env *env, char *key);
void	ft_env_set(t_env **env, char *key, char *value);
void	ft_env_unset(t_env **env, char *key);

// ====Lexer===
t_token	*ft_token_new(t_token_type type, char *value, char quote);
void	ft_token_add_back(t_token **lst, t_token *new);
char	*ft_token_value_dup(const char *src);
int		ft_token_list_size(t_token *lst);
t_token	*ft_lex_word(const char *input, int *i);
t_token	*ft_lexer(const char *input);

// ====Parser===
t_cmd	*ft_parser(t_token *tok);
int		ft_syntax_check(t_token *tok);

// ====Expand===
void	ft_expand(t_cmd *cmds, t_env *env);

// ====Builtins===
int		builtin_echo(t_cmd *cmd);
int		builtin_env(t_env *env);
int		buitin_exit(t_cmd *cmd, t_shell *shell);
int		buitin_pwd(void);
int		buitin_cd(t_cmd *cmd, t_shell *shell);

//Utils
int		ft_isspace(char c);
int		ft_isoper(char c);
int		ft_iswordend(char c, char quote);
void	ft_print_tokens(t_token *tokens);
void	ft_print_cmds(t_cmd *cmds);
t_token	*print_err_syntax(void);
char	*ft_gc_strjoin(const char *s1, const char *s2);

#endif