#include "minishell.h"
#include "parsing.h"
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	while (1)
	{
		line = readline("Here is my prompt:");
		if (line)
			printf("Here is the line: <%s>\n", line);
		else
		{
			printf("Readline is NULL\n");
			//free everything before exiting
			exit(EXIT_FAILURE);
		}
		if (line && *line)
			add_history(line);
		if (is_there_unclosed_quotes(line))
		{
			printf("There are unclosed quotes\n");
			free(line);
			continue ;
		}
		t_token *tokens = scanner(line);
		expand_variables(envp, tokens);
		merge_tokens(tokens);
		print_tokens(tokens);
		t_node *nodes = parser(tokens);
		print_nodes(nodes, 0);
		free_tokens(tokens);
		free_nodes(nodes);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
