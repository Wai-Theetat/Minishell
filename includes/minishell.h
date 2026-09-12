/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: koonchevychpai123 <koonchevychpai123@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 16:02:02 by tdharmar          #+#    #+#             */
/*   Updated: 2026/09/13 12:00:00 by koonchevych      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "structs.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
/* readline.h uses FILE, so it must come after <stdio.h> */
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef HOSTNAME
#  define HOSTNAME "host"
# endif

// ====Signals===
extern volatile sig_atomic_t	g_signal;
void							set_signal(int signum, void (*handler)(int));
void							prompt_sigint(int sig);
void							set_prompt_signals(void);
void							set_exec_signals(void);
void							set_child_signals(void);
void							print_signal_msg(int status);

// ====Env===
t_env							*ft_env_new(char *key, char *value);
void							ft_env_add_back(t_env **env,
									t_env *new_env_node);
t_env							*ft_env_from_line(char *line);
t_env							*ft_env_init(char **envp);
void							ft_env_list(t_env **envp);
char							**ft_env_to_char(t_env *env);
char							*ft_env_get(t_env *env, char *key);
void							ft_env_set(t_env **env, char *key, char *value);
void							ft_env_mark(t_env **env, char *key);
t_env							*ft_env_reorder(t_env *env);
void							ft_env_unset(t_env **env, char *key);

// ====Lexer===
t_token							*ft_token_new(t_token_type type, char *value,
									char quote);
void							ft_token_add_back(t_token **lst, t_token *new);
char							*ft_token_value_dup(const char *src);
int								ft_token_list_size(t_token *lst);
t_token							*ft_lex_word(const char *input, int *i);
t_token							*ft_lexer(const char *input);
const char						*token_text(t_token *tok);

// ====Parser===
t_node							*ft_parser(t_token *tok);
t_node							*node_new(t_node_type type);
t_node							*parse_list(t_parse *p);
t_node							*parse_pipeline(t_parse *p);
t_node							*parse_command(t_parse *p);
t_node							*parse_arith(t_parse *p);
t_node							*parse_subshell(t_parse *p);
t_node							*parse_simple(t_parse *p);
void							parse_redir(t_parse *p, t_token **redirs);
t_token							*dup_word(t_token *src, t_token_type type);
int								is_redir_type(t_token_type type);
void							syntax_error(t_parse *p, t_token *tok);
int								ft_heredoc(const char *delim, int expand,
									t_env *env, int exit_code);
void							heredoc_child(int *pipefd, t_hdoc *h);
int								ast_run_heredocs(t_node *node, t_env *env,
									int exit_code);
void							ast_close_heredocs(t_node *node);
int								redir_run_heredocs(t_token *redirs, t_env *env,
									int exit_code);

// ====Expand===
void							ft_expand_cmd(t_cmd *cmd, t_env *env,
									int exit_code);
void							ft_expand_redirs(t_token *redirs, t_env *env,
									int exit_code);
char							*expand_str(const char *str, t_env *env,
									int exit_code);
t_wstr							expand_word(const char *str, const char *mask,
									t_env *env, int exit_code);
void							wstr_init(t_wstr *w);
void							wstr_addc(t_wstr *w, char c, char m);
void							wstr_addstr(t_wstr *w, const char *s, char m);
int								split_fields(t_wstr *w, t_wstr *out, int max);
void							argv_init(t_argv *a);
void							argv_push(t_argv *a, char *s);
char							**glob_word(t_wstr *field, int *count);
int								has_wildcard(t_wstr *field);
int								ft_collate_cmp(const char *a, const char *b);

// ====Exec===
char							*find_exec(char *cmd, t_env *envp);
int								exec_not_found_code(char *cmd);
int								exec_node(t_node *node, t_shell *shell);
void							child_exec_node(t_node *node, t_shell *shell);
int								exec_simple(t_cmd *cmd, t_env *envp,
									t_shell *shell);
void							exec_each_cmd(t_cmd *cmd, t_shell *shell);
int								exec_pipeline(t_node *node, t_shell *shell);
int								exec_subshell(t_node *node, t_shell *shell);
int								exec_arith(t_node *node, t_shell *shell);
int								apply_redir_list(t_token *redirs);
int								run_redir_only(t_cmd *cmd);
int								pipe_status_code(int status);
int								wait_last(pid_t last, pid_t *pids, int count);

// ====Builtins===
int								builtin_echo(t_cmd *cmd);
int								builtin_env(t_env *env);
int								builtin_export(t_cmd *cmd, t_shell *shell);
int								builtin_exit(t_cmd *cmd, t_shell *shell);
int								builtin_pwd(void);
int								builtin_cd(t_cmd *cmd, t_shell *shell);
int								builtin_unset(t_cmd *cmd, t_shell *shell);
int								export_list(t_env *env);
int								is_builtin(char *cmd_name);
int								run_builtin(t_cmd *cmd, t_shell *shell);
int								run_builtin_redir(t_cmd *cmd, t_shell *shell);

// Utils
int								ft_isspace(char c);
int								ft_isoper(const char *s, int i);
int								ft_iswordend(const char *s, int i, char quote);
void							write_msh_error(char *err_message);
void							write_exec_error(char *exec_name,
									char *err_message);
void							write_msh_exec_error(char *exec_name,
									char *err_message);
void							write_msh_exec_arg_error(char *exec_name,
									char *arg, char *err_message);
void							write_msh_exec_arg_error_nocolon(
									char *exec_name, char *arg,
									char *err_message);
void							ft_print_tokens(t_token *tokens);
void							ft_print_cmds(t_cmd *cmds);
t_token							*print_err_syntax(void);
char							*ft_gc_strjoin(const char *s1, const char *s2);
char							*ft_gc_itoa(int n);
char							*ft_gc_strdup(const char *s);
char							*read_full_input(char *first);
int								shell_interactive(void);

#endif
