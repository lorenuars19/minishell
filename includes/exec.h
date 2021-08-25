/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 13:43:36 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 14:36:30 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# define FORKED_CHILD 0

typedef struct s_pipe_context
{
	int	fd[2];
	int	fd_close;
}	t_context;

int		exec(t_node *node);
int		exec_node(t_node *node, t_context *ctx);
int		exec_builtin(t_node *node);

int		builtin_echo(char *argv[]);
int		builtin_cd(char *argv[]);
int		builtin_pwd(char *argv[]);
int		builtin_export(char *argv[]);
int		builtin_unset(char *argv[]);
int		builtin_env(char *argv[]);
int		builtin_exit(char *argv[]);

t_bool	is_command_a_builtin(t_node *node);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
void	sigquit_handler_exec(int signum);
void	sigint_handler_interactive(int signum);
void	sigint_handler_exec(int signum);
void	eof_exit(void);
char	**realloc_envp_with_one_more_line(void);
int		print_sorted_envp(char **envp);
t_bool	is_a_valid_exported_name(char *name);
void	print_export_name_error(char *name);
t_bool	is_line_empty(char *line);
int		print_error_filename(char *filename);
int		set_redirection(t_node *node, t_context *ctx);
int		get_heredocs(t_node *node);
t_bool	is_command_a_builtin(t_node *node);
void	wait_for_children(t_node *node, t_bool is_end_of_pipeline);
char	*get_bin_filename(char *name);
int		exec_builtin_in_parent(t_node *node, t_context *ctx);
t_bool	there_is_a_heredoc_redirection(t_node *node);
t_bool	is_part_of_pipeline(t_context *ctx);
void	destroy_strs(char **strs, int i);
void	sort_envp(char **envp);

#endif
