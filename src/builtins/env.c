#include "minishell.h"

int	builtin_env(char *argv[])
{
	int	i;

	(void)argv;
	i = 0;
	while (g_shell.envp[i])
	{
		printf("%s\n", g_shell.envp[i]);
		i++;
	}
	return (0);
}
