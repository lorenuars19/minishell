#include "minishell.h"

int builtin_echo(char *argv[], char *envp[])
{
	(void)envp;
	int i;

	i = 1;

	while (argv && argv[i])
	{
		put_str(argv[i]);
		i++;
	}
	put_chr('\n');
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
	char buf[BUFF_SIZ + 1];

	if (!getcwd(buf, BUFF_SIZ))
		return (1);
	put_str_nl(buf);
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
