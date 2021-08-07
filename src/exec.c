#include <stdlib.h>
#include "minishell.h"

/*
** WRAPPER to facilitate memory management
*/

int	execution(t_node *node, char *envp[])
{
	t_exdat	ed;

	ed = (t_exdat){0, BINARY, YES_FORK, builtin_dummy, NOT_PIPE, {0, 0}, -1};
	ed.status = exec_nodes(&ed, node, envp);
	if (ed.status)
		return (ed.status);
	return (0);
}

/*
** The actual starting point that will be called recursively
*/

int	exec_nodes(t_exdat *ed, t_node *node, char *envp[])
{
	if (node->type == COMMAND_NODE)
	{
		ed->status = exec_command(ed, node, envp);
		if (ed->status)
			return (ed->status);
	}
	else if (node->type == PIPE_NODE)
	{
		ed->status = exec_piped(ed, node, envp);
		if (ed->status)
			return (ed->status);
	}
	else
	{
		return (1);
	}
	return (0);
}
/*
** BUILTINS
*/

static t_builtin_f get_builtin(int index)
{
	static t_builtin_f	builtins[] = {
		builtin_echo,
		builtin_cd,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
		builtin_dummy
		};
	if (index >= 0 && index < BUILTIN_MAX)
	{
		return(builtins[index]);
	}
	return (builtin_dummy);
}

static void	check_for_builtins(t_exdat *ed, t_node *node)
{
	int i;
	static char *builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL};
	i = 0;
	while (node->args && node->args[0] && builtins[i] \
		&& str_cmp(node->args[0], builtins[i]))
		i++;

	if (i == BUILTIN_CD || i == BUILTIN_EXPORT || i == BUILTIN_UNSET || i == BUILTIN_EXIT)
		ed->fork_or_not = NOT_FORK;

	ed->f_to_call = get_builtin(i);

	if (i >= 0 && i < BUILTIN_MAX)
		ed->builtin_mode = BUILTIN;
}

static pid_t g_cpid;

void	sig_handle_child(int signum)
{
	kill(g_cpid, signum);
}

void	sig_handle_parent(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	printf("\033[34m");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	setup_signals(t_revert revert_to_default)
{
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			return (error_sys_put("signal"));
	if (revert_to_default == REVERT_TO_DEFAULT)
	{
		if (signal(SIGINT, sig_handle_parent) == SIG_ERR)
			return (error_sys_put("signal"));
	}
	else
	{
		if (signal(SIGINT, sig_handle_child) == SIG_ERR)
			return (error_sys_put("signal"));
	}
	return (0);
}

int	sub_set_redir(t_exdat *ed, t_node *node)
{
	t_redirection	*redir;
	int				o_flags;
	int				dup_fd;

	redir = node->redirections;
	while (redir)
	{
		if (redir && redir->next && close(ed->fd_to_close))
			return (error_printf("close : %d %s\n",
				ed->fd_to_close, strerror(errno)));
		o_flags = 0;
		dup_fd = STDOUT_FILENO;
		if (redir->mode == M_INPUT)
		{
			o_flags = O_RDONLY;
			dup_fd = STDIN_FILENO;
		}
		else if (redir->mode == M_TRUNCATE)
			o_flags = O_TRUNC | O_WRONLY | O_CREAT;
		else if (redir->mode == M_APPEND)
			o_flags = O_APPEND | O_WRONLY | O_CREAT;
		ed->fd_to_close = open(redir->filename, o_flags);
		if (ed->fd_to_close < 0)
			return (error_sys_put("open"));
		if (dup2(ed->fd_to_close, dup_fd) < 0)
			return (error_sys_put("dup2"));
		redir = redir->next;
	}
	return (0);
}

int	set_redirection(t_exdat *ed, t_node *node)
{
	if (!ed || !node)
		return (1);
	if (ed->need_pipe == YES_PIPE
		&& dup2(ed->p[STDIN_FILENO], STDIN_FILENO) < 0)
		return (error_sys_put("dup2"));
	if (ed->need_pipe == YES_PIPE
		&& dup2(ed->p[STDOUT_FILENO], STDOUT_FILENO) < 0)
		return (error_sys_put("dup2"));
	if (ed->fd_to_close >= 0)
	{
		if (close(ed->fd_to_close))
			return (error_sys_put("close"));
	}
	if (sub_set_redir(ed, node))
		return (error_sys_put("sub_set_redir"));
	return (0);
}

int	exec_command(t_exdat *ed, t_node *node, char *envp[])
{

	*ed = (t_exdat){0, BINARY, YES_FORK, builtin_dummy, ed->need_pipe,
		{ed->p[0], ed->p[1]}, ed->fd_to_close};
	check_for_builtins(ed, node);

	g_cpid = 0;
	if (ed->fork_or_not == YES_FORK)
	{
		g_cpid = fork();

	}
	if (g_cpid < 0)
		return (error_sys_put("fork"));
	else if (g_cpid == FORKED_CHILD)
	{

//TODO REMOVE DEBUG
printf("\033[32;1mEXEC_DATA\033[0m : ed->builtin_mode %s\033[0m ed->fork_or_not %s\033[0m ed->f_to_call <%p>\n", \
		(ed->builtin_mode == BUILTIN) ? ("\033[33mBUILTIN") : ("\033[33mBINARY"),\
		(ed->fork_or_not == YES_FORK) ? ("\033[33mFORKED") : ("\033[33mNOT FORKED"), \
		ed->f_to_call);
		if (set_redirection(ed, node))
			return (error_sys_put("set_redirection"));

		if (ed->builtin_mode == BUILTIN)
		{
			ed->status = ed->f_to_call(node->args, envp);
		}
		else
		{
			ed->status = exec_binary(node, envp);
		}
		return (ed->status);
	}
	else
	{
		ed->status = wait_for_child(g_cpid);
		if (close(ed->fd_to_close))
		if (setup_signals(REVERT_TO_DEFAULT))
			return (error_sys_put("setup_signals"));
		if (ed->status)
			return (ed->status);
	}
	return (0);
}

/*
** CHILD PROCESS MANAGEMENT
*/

int	sub_wait_for_child(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (wstatus);
}

int	wait_for_child(pid_t cpid)
{
	pid_t	w;
	int		ret;
	int		wstatus;

	w = waitpid(cpid, &wstatus, WUNTRACED);
	if (w < 0)
		return (error_sys_put("wait"));
	ret = sub_wait_for_child(wstatus);
	if (ret)
		return (ret);
	while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
	{
		w = waitpid(cpid, &wstatus, WUNTRACED);
		ret = sub_wait_for_child(wstatus);
		if (ret)
			return (ret);
	}
	return (ret);
}

/*
** Finding Binary in $PATH
*/

static int	is_path_executable(char *path)
{
	struct stat	sb;

	if (!path)
		return (0);
	if (stat(path, &sb))
		return (0);
	if (S_ISREG(sb.st_mode) && sb.st_mode & 0111)
	{
		return (1);
	}
	return (0);
}

static void	free_find_path(char	***tab)
{
	int	i;

	i = 0;
	while (tab && *tab && **tab && **tab[i])
	{
		free(**tab[i]);
	}
	free(*tab);
}

static char	*find_path(t_node *node)
{
	char	*path;
	char	**tab;
	int		i;

	if (node->args && node->args[0] && is_path_executable(node->args[0]))
		return (node->args[0]);
	path = getenv("PATH");
	tab = str_split(path, ":");
	i = 0;
	while (tab && tab[i] && node->args && node->args[0])
	{
		path = str_jointo(tab[i], "/", NULL);
		path = str_jointo(path, node->args[0], &path);
		if (is_path_executable(path))
		{
			free_find_path(&tab);
			return (path);
		}
		free(path);
		i++;
	}
	free_find_path(&tab);
	return (NULL);
}

int	exec_binary(t_node *node, char *envp[])
{
	char	*path;
	int		status;

	status = 0;
	path = find_path(node);
	if (!path && node->args)
		return (error_printf(1, "command \"%s\" not found", node->args[0]));

	//TODO remove debug
	printf("\033[32;1mexecve\033[0m(path \"%s\", node->args <%p>, envp <%p>) \n\033[34m>->->->->->->->->\033[0m\n", path, node->args, envp);

	if (setup_signals(DEFER_SIGNAL))
			return (error_sys_put("setup_signals"));

	status = execve(path, node->args, envp);

	free(path);

	return (status);
}

/*
** PIPES EXEC
*/

int	exec_piped(t_exdat *ed, t_node *node, char *envp[])
{
	if(pipe(ed->p))
		return (error_sys_put("pipe"));
	ed->need_pipe = YES_PIPE;
	ed->fd_to_close = ed->p[STDIN_FILENO];
	ed->status = exec_nodes(ed, node->left, envp);
	if (ed->status)
		return (ed->status);
	ed->fd_to_close = ed->p[STDOUT_FILENO];
	ed->status = exec_nodes(ed, node->right, envp);
	if (ed->status)
		return (ed->status);
	if (close(ed->p[STDIN_FILENO]))
		return (error_sys_put("close"));
	if (close(ed->p[STDOUT_FILENO]))
		return (error_sys_put("close"));
	return (ed->status);
}
