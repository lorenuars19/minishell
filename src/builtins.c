#include "minishell.h"

int builtin_echo(char *argv[], char *envp[])
{
	int		i;
	t_bool	should_print_trailing_nl;

	(void)envp;
	if (*argv && argv[1] && str_cmp(argv[1], "-n") == 0)
	{
		should_print_trailing_nl = FALSE;
		i = 2;
	}
	else
	{
		should_print_trailing_nl = TRUE;
		i = 1;
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
	(void)argv;
	return (0);
}

int builtin_pwd(char *argv[], char *envp[])
{
	(void)argv;
	(void)envp;

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
	(void)envp;
	(void)argv;
	return (0);
}

int builtin_exit(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	return (0);
}
