#include "minishell.h"

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

t_bool	is_line_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;

	(void)argv;
	(void)argc;
	g_info.envp = make_envp_copy(envp);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("$>");
		if (!line)
			eof_exit();
		if (line && !is_line_empty(line))
			add_history(line);
		tokens = scanner(line);
		if (syntax_checker(line, tokens) != 0)
		{
			free_tokens_incl_data(tokens);
			free(line);
			continue ;
		}
		free(line);
		if (expand_variables(g_info.envp, tokens) != 0)
		{
			free_tokens_incl_data(tokens);
			continue ;
		}
		if (merge_tokens(tokens) != 0)
		{
			free_tokens_incl_data(tokens);
			continue ;
		}
		g_info.nodes = parser(tokens);
		free_tokens_excl_data(tokens);
		if (!g_info.nodes)
			continue ;
		exec(g_info.nodes);
		free_nodes(g_info.nodes);
	}
}
