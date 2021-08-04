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
	if (argv && chdir(argv[1]))
	{
		return (error_sys_put(errno));
	}
	return (0);
}

int builtin_pwd(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;

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
	int i;

	(void)argv;
	i = 0;
	while (envp && envp[i])
	{
		put_str_nl(envp[i]);
		i++;
	}
	return (0);
}

int builtin_exit(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	exit(0);
	return (0);
}
