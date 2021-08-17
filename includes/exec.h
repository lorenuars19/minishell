#ifndef EXEC_H
# define EXEC_H

# define BUFF_SIZ	1024

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
	BUILTIN_END
}	t_builtins;

typedef struct s_pipe_context
{
	int	fd[2];
	int	fd_close;
}	t_context;

int	exec(t_node *node, char *envp[]);

int exec_node(t_node *node, t_context *ctx, char *envp[]);

int exec_command(t_node *node, t_context *ctx, char *envp[]);
int exec_pipe(t_node * node, t_context *ctx, char *envp[]);

int wait_for_child(pid_t cpid);

int	exec_binary(t_node *node, char *envp[]);
char *find_path(t_node *node);
int	is_path_executable(char *path);


int builtin_echo(char *argv[], char *envp[]);
int builtin_cd(char *argv[], char *envp[]);
int builtin_pwd(char *argv[], char *envp[]);
int builtin_export(char *argv[], char *envp[]);
int builtin_unset(char *argv[], char *envp[]);
int builtin_env(char *argv[], char *envp[]);
int builtin_exit(char *argv[], char *envp[]);

t_bool is_command_a_builtin(t_node *node);
int exec_builtin(t_node *node, char **envp);


#endif
