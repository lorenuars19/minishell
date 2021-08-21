#include "minishell.h"
#include <stdlib.h>


t_info	g_info;

void	sigint_handler(int signum)
{
	(void)signum;
	if (g_info.is_exec_ongoing)
		return ;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int signum)
{
	(void)signum;
	return ;
}

void	eof_exit(void)
{
	free_envp(g_info.envp);
	put_str_fd_nl(STDERR_FILENO, "exit");
	exit(0);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	g_info.envp = make_envp_copy(envp);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	while (1)
	{
		line = readline("$>");
		if (!line)
			eof_exit();
		if (line && *line)
			add_history(line);
		t_token *tokens = scanner(line);
		if (syntax_checker(line, tokens) != 0)
		{
			free_tokens_incl_data(tokens);
			free(line);
			continue ;
		}
		free(line);
		expand_variables(g_info.envp, tokens);
		merge_tokens(tokens);
		// print_tokens(tokens);
		t_node *nodes = parser(tokens);
		free_tokens_excl_data(tokens);
		g_info.nodes = nodes;
		// print_nodes(nodes, 0);
		exec(nodes);
		free_nodes(nodes);
	}
}
