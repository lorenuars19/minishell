#include "minishell.h"


void	sigint_handler_interactive(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler_exec(int signum)
{
	(void)signum;
	return ;
}

void	sigquit_handler_exec(int signum)
{
	(void)signum;
	return ;
}

void	eof_exit(void)
{
	free_envp(g_shell.envp);
	put_str_fd_nl(STDERR_FILENO, "exit");
	exit(0);
}
