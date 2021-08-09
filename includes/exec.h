#ifndef EXEC_H
# define EXEC_H

# define BUFF_SIZ	1024

# define WRIT_P 1
# define READ_P 0

# define FORKED_CHILD 0
typedef enum e_builtin_commands
{
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_MAX
}	t_builtins;

typedef	int (*t_builtin_f)(char *argv[], char *envp[]);
typedef struct s_exec_data
{
	int				status;
	t_bool			is_builtin;
	t_bool			is_fork;
	t_builtin_f		f_to_call;

	t_bool			is_pipe;
	int				p[2];
	int				fd_close;
	int				n_children;
}	t_exdat;

typedef enum e_revert_signal_or_not
{
	DEFER_SIGNAL,
	REVERT_TO_DEFAULT
}	t_revert;

int		execution(t_node *node, char *envp[]);
int		exec_nodes(t_exdat *ed, t_node *node, char *envp[]);


int		exec_command(t_exdat *ed, t_node *node, char *envp[]);
int 	wait_for_child(pid_t cpid);
void	check_for_builtins(t_exdat *ed, t_node *node);
int		setup_signals(t_revert revert_to_default);

int		exec_binary(t_node *node, char *envp[]);



int		exec_piped(t_exdat *ed, t_node *node, char *envp[]);
int		set_redir_file(t_exdat *ed, t_node *node);
int		set_redir_pipe(t_exdat *ed, t_node *node);

int		filter_wstatus(int wstatus);
int		is_path_regular_file(char *path);
int		is_path_executable(char *path);

/*
** BUILTINS
*/

int		builtin_echo(char *argv[], char *envp[]);
int		builtin_cd(char *argv[], char *envp[]);
int		builtin_pwd(char *argv[], char *envp[]);
int		builtin_export(char *argv[], char *envp[]);
int		builtin_unset(char *argv[], char *envp[]);
int		builtin_env(char *argv[], char *envp[]);
int		builtin_dummy(char *argv[], char *envp[]);

#endif
