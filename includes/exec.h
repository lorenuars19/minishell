#ifndef EXEC_H
# define EXEC_H

#define FORKED_CHILD 0

typedef struct s_pipe_context
{
	int	fd[2];
	int	close_fd;
}	t_ctx;

int	execution(t_node *node, char *envp[]);

int exec_nodes(t_node *node, t_ctx * ctx, char *envp[]);

int exec_command(t_node *node, char *envp[]);
int wait_for_child(pid_t cpid);
int check_for_builtins(t_node *node, char *envp[]);
int	exec_builtin(t_node *node, char *envp[]);

int exec_piped(t_node * node, t_ctx *ctx, char *envp[]);

#endif
