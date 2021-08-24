#include "minishell.h"

int	builtin_cd(char *argv[])
{
	char	*path;

	if (!argv[1])
		path = get_value_from_envp("HOME", g_shell.envp);
	else if (argv[2])
	{
		put_str_fd(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
		path = str_dupli(argv[1]);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		put_str_fd(STDERR_FILENO, "minishell: cd: ");
		put_str_fd(STDERR_FILENO, path);
		put_str_fd(STDERR_FILENO, ": ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		free(path);
		return (1);
	}
	free(path);
	return (0);
}
