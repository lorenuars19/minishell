#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int builtin_echo(char *argv[], char *envp[])
{
	int		i;
	t_bool	should_print_trailing_nl;

	(void)envp;
	i = 1;
	should_print_trailing_nl = TRUE;
	if (*argv && argv[1] && str_cmp(argv[1], "-n") == 0)
	{
		should_print_trailing_nl = FALSE;
		i = 2;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (should_print_trailing_nl)
		printf("\n");
	return (0);
}

int builtin_cd(char *argv[], char *envp[])
{
	(void)envp;

	char *path;

	if (argv && !argv[1])
		path = getenv("HOME");
	else if (argv && argv[1])
		path = argv[1];
	if (chdir(path))
	{
		return (error_sys_put("chdir"));
	}
	return (0);
}

int builtin_pwd(char *argv[], char *envp[])
{
	char	*pwd;

	(void)argv;
	(void)envp;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int builtin_export(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	return (0);
}

int builtin_unset(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	return (0);
}

int builtin_env(char *argv[], char *envp[])
{
	int i;

	(void)argv;
	i = 0;
	while (envp && envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	builtin_dummy(char *argv[], char *envp[])
{
	(void)argv;
	(void)envp;
	return (0);
}
