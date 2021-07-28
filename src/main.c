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
		if (line && *line)
			add_history(line);
		// char *str = ft_strdup_set(line, SPECIAL_CHARS);
		// printf("<%s>\n", str);
		// free(str);
		t_token *tokens = scanner(line);
		expand_variables(envp, tokens);
		print_tokens(tokens);
		t_node *nodes = parser(tokens);
		print_nodes(nodes, 0);
		free_tokens(tokens);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
