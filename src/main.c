#include "minishell.h"
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	int 	status;
	char 	*line;
	char	*prompt;

	status = 0;

	if (setup_signals(REVERT_TO_DEFAULT))
			return (error_sys_put("setup_signals"));

	while (1)
	{
		line = NULL;
		prompt = NULL;
		prompt = getcwd(NULL, 0);
		prompt = str_jointo("\n\033[34m", prompt, &prompt);
		if (status)
			prompt = str_jointo(prompt, " \033[31;1m$\033[0m ", &prompt);
		else
			prompt = str_jointo(prompt, " \033[32;1m$\033[0m ", &prompt);
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			printf("\nexit\n");
			free(line);
			return (EXIT_SUCCESS);
		}
		printf("\nline : \"%s\"\n", line);
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

		status = execution(nodes, envp);

//TODO remove debug
dprintf(2, "\n> Last command status : %d <\n", status);

		free_tokens_without_data(tokens);
		free_nodes(nodes);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
			return (EXIT_SUCCESS);
		}
		free(line);
	}
	return (0);
}
