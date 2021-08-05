#include "minishell.h"
#include <stdlib.h>


int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *line = NULL;

	while (1)
	{
		printf("\033[34m");
		builtin_pwd(argv, envp);
		line = readline("$ \033[32m>\033[0m ");
		if (!line)
		{
			printf("Readline is \033[31;1mNULL\033[0m\n");
			free(line);
			exit(EXIT_FAILURE);
		}
		printf("\nline: \"%s\"\n", line);
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

		execution(nodes, envp);

		free_tokens_without_data(tokens);
		free_nodes(nodes);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
printf("\033[32;1mMINISHELL KILLED FROM MAIN\033[0m\n");
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
