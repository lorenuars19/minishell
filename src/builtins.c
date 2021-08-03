#include "minishell.h"

int builtin_echo(char *argv[], char *envp[])
{
	(void)envp;
	int i;

	i = 1;

	while (argv && argv[i])
	{
		put_str_fd(1, argv[i]);
		i++;
	}
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
	(void)envp;
	(void)argv;
	return (0);
}

int builtin_export(char *argv[], char *envp[])
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
