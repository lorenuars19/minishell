#include "minishell.h"
#include <stdlib.h>


int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	while (1)
	{
		builtin_pwd(argv, envp);
		line = readline("\033[32;1m>\033[0m ");
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
		t_token *tokens = scanner(line);
		if (syntax_checker(line, tokens) != 0)
		{
			free_tokens_with_data(tokens);
			free(line);
			continue ;
		}
		expand_variables(envp, tokens);
		merge_tokens(tokens);
		print_tokens(tokens);
		t_node *nodes = parser(tokens);
		print_nodes(nodes, 0);

		//TODO Excution
		execution(nodes, envp);

		free_tokens_without_data(tokens);
		free_nodes(nodes);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
