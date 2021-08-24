#ifndef EXEC_H
# define EXEC_H

# define FORKED_CHILD 0

typedef struct s_pipe_context
{
	int	fd[2];
	int	fd_close;
}	t_context;

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
char *ft_itoa(int n);

void sigquit_handler_exec(int signum);
void	sigint_handler_interactive(int signum);
void	sigint_handler_exec(int signum);

#endif
