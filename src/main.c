#include "minishell.h"
#include "parsing.h"
#include <stdlib.h>

int main(int argc, char **argv)
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
		char *str = ft_strcdup(line, T_DQUOTE);
		put_str(str);
		free(str);
		if (str_cmp("exit", line) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		free(line);
	}
}
