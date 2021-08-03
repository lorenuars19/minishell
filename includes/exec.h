#ifndef EXEC_H
# define EXEC_H

#define FORKED_CHILD 0

typedef struct s_pipe_context
{
	int	fd[2];
	int	close_fd;
}	t_ctx;

int	execution(t_node *nodes);


#endif
