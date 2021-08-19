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

typedef struct
{
	char	**envp;
	t_node	*nodes;
	int		last_exit_status;
}	t_info;

int	exec(t_node *node);

int exec_node(t_node *node, t_context *ctx);

int exec_command(t_node *node, t_context *ctx);
int exec_pipe(t_node * node, t_context *ctx);

int builtin_echo(char *argv[]);
int builtin_cd(char *argv[]);
int builtin_pwd(char *argv[]);
int builtin_export(char *argv[]);
int builtin_unset(char *argv[]);
int builtin_env(char *argv[]);
int builtin_exit(char *argv[]);

t_bool is_command_a_builtin(t_node *node);
int exec_builtin(t_node *node);
int ft_strncmp(const char *s1, const char *s2, size_t n);

char **ft_split(char const *s, char c);

#endif
