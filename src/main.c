#include "minishell.h"
#include <stdlib.h>


t_info	g_info;

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	g_info.envp = make_envp_copy(envp);
	while (1)
	{
		line = readline("$>");
		if (!line)
		{
			printf("Readline is NULL\n");
			//TODO free everything before exiting
			exit(EXIT_FAILURE);
		}
		if (line && *line)
			add_history(line);
		t_token *tokens = scanner(line);
		if (syntax_checker(line, tokens) != 0)
		{
			free_tokens_with_data(tokens);
			free(line);
			continue ;
		}
		free(line);
		expand_variables(g_info.envp, tokens);
		merge_tokens(tokens);
		// print_tokens(tokens);
		t_node *nodes = parser(tokens);
		free_tokens_without_data(tokens);
		g_info.nodes = nodes;
		// print_nodes(nodes, 0);
		exec(nodes);
		free_nodes(nodes);
	}
}
