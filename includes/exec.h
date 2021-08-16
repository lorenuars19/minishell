#ifndef EXEC_H
# define EXEC_H

# define BUFF_SIZ	1024

# define WRIT_P 1
# define READ_P 0

# define FORKED_CHILD 0

# define _TO_STR(VAL) #VAL
# define STRYFY(VAL) _TO_STR(VAL)

# define _STR_ALL_ __FILE__ ":" STRYFY(__LINE__)
# define _DEB_LOC _STR_ALL_

# define DED \
dprintf(2, "\n>>>\n\033[32;1m%s\033[0m : cmd [%s] node->type \033[33m%s\033[0m | "\
		"ed->builtin_mode %s\033[0m ed->fork_or_not %s\033[0m ed->f_to_call <%p> |\n"\
		"is_pipe %d pipe_open %d n_children %d |\nctx fd[0] %d fd[1] %d fd_close %d\n<<<\n", \
		_DEB_LOC , \
		(node->args) ? (node->args[0]) : ("|"),\
		(node->type == COMMAND_NODE) ? ("COMMAND_NODE"): ("PIPE_NODE"),\
		(ed->is_builtin == TRUE) ? ("\033[33mBUILTIN") : ("\033[33mBINARY"),\
		(ed->is_fork == TRUE) ? ("\033[33mFORKED") : ("\033[33mNOT FORKED"), \
		ed->f_to_call, ed->is_pipe, ed->pipe_open, ed->n_children,\
		ctx->fd[0], ctx->fd[1], ctx->fd_close);


# define DCTX(value) dprintf(2, "%s " #value " : fd[0] %d fd[1] %d fd_close %d | node->args[0] %s\n",\
	_DEB_LOC, value.fd[0], value.fd[1], value.fd_close, (node->args) ? (node->args[0]) : ("|"));

# define DPCTX(value) dprintf(2, "%s " #value " : fd[0] %d fd[1] %d fd_close %d | node->args[0] %s\n",\
	_DEB_LOC, value->fd[0], value->fd[1], value->fd_close, (node->args) ? (node->args[0]) : ("|"));

# define DCL(value) dprintf(2, "\n> %s : \033[31mCLOSE FD " #value " %d \033[0m<\n", _DEB_LOC, value);

# define close(value) DCL(value) close(value);

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

typedef struct s_pipe_context
{
	int		fd[2];
	int		fd_close;
}	t_ctx;
typedef struct s_exec_data
{
	char 		**envp;

	int			status;

	t_bool		is_builtin;
	t_bool		is_fork;

	t_builtin_f	f_to_call;

	t_bool		is_pipe;
	t_bool		pipe_open;

	int			n_children;

	int			file_close;
}	t_exdat;



typedef enum e_revert_signal_or_not
{
	DEFER_SIGNAL,
	REVERT_TO_DEFAULT
}	t_revert;

int		execution(t_node *node, char *envp[]);
int 	exec_nodes(t_exdat *ed, t_node *node, t_ctx *ctx);


int		exec_command(t_exdat *ed, t_node *node, t_ctx *ctx);
int		wait_for_child(void);
void	check_for_builtins(t_exdat *ed, t_node *node);
int		setup_signals(t_revert revert_to_default);

int		exec_binary(t_node *node, char *envp[]);

int		exec_pipe(t_exdat *ed, t_node *node, t_ctx *ctx);
int		set_redir_file(t_exdat *ed, t_node *node);
int		set_redir_pipe(t_exdat *ed, t_ctx *ctx, t_node *node);


int		get_exit_code(int wstatus);
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
