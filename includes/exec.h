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

typedef enum e_builtin_mode
{
	BINARY,
	BUILTIN
}	t_builtin_mode;
typedef enum e_fork_or_not
{
	NOT_FORK,
	YES_FORK
}	t_fork_or_not;

typedef enum e_need_pipe
{
	NOT_PIPE,
	YES_PIPE
}	t_need_pipe;
typedef struct s_exec_data
{
	int				status;
	t_builtin_mode	builtin_mode;
	t_fork_or_not	fork_or_not;
	t_builtin_f		f_to_call;
	t_need_pipe		need_pipe;
	int				p[2];
	int				fd_to_close;
	int				n_children;
}	t_exdat;

typedef enum e_revert_signal_or_not
{
	DEFER_SIGNAL,
	REVERT_TO_DEFAULT
}	t_revert;


int	execution(t_node *node, char *envp[]);
int	exec_nodes(t_exdat *ed, t_node *node, char *envp[]);


int	exec_command(t_exdat *ed, t_node *node, char *envp[]);
int wait_for_child(pid_t cpid);

int	exec_binary(t_node *node, char *envp[]);

int	exec_builtin(t_node *node, char *envp[], int index);

/*
** BUILTINS
*/

int builtin_echo(char *argv[], char *envp[]);
int builtin_cd(char *argv[], char *envp[]);
int builtin_pwd(char *argv[], char *envp[]);
int builtin_export(char *argv[], char *envp[]);
int builtin_unset(char *argv[], char *envp[]);
int builtin_env(char *argv[], char *envp[]);
int	builtin_dummy(char *argv[], char *envp[]);

int	exec_piped(t_exdat *ed, t_node *node, char *envp[]);

int	setup_signals(t_revert revert_to_default);

int	filter_wstatus(int wstatus);

#endif
