#include <stdlib.h>
#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	while (1)
	{
		line = readline("\033[32;1mMINISHELL \033[0m\033[33;1m>\033[0m ");
		if (line)
			printf("Input : \"%s\"\n", line);
		if (line && *line)
			add_history(line);
		// char *str = ft_strdup_set(line, SPECIAL_CHARS);
		// printf("<%s>\n", str);
		// free(str);
		if (is_there_unclosed_quotes(line))
		{
			free(line);
			continue ;
		}
		t_token *tokens = scanner(line);
		expand_variables(envp, tokens);
		merge_tokens(tokens);
		print_tokens(tokens);
		t_node *nodes = parser(tokens);
		print_nodes(nodes, 0);
		// exec
		exec_nodes(nodes, NULL);


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
