#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

// int exec_command(t_node *node, char *envp[])
// {
// 	if (node->type != COMMAND_NODE || node->args == NULL)
// 		return (0);
// 	if (node->args[0] && strcmp(node->args[0], "echo") == 0)
// 	{
// 		builtin_echo(node->args, envp);
// 	}
// 	else if (node->args[0] && strcmp(node->args[0], "env") == 0)
// 	{
// 		builtin_env(node->args, envp);
// 	}
// 	else if (node->args[0] && strcmp(node->args[0], "pwd") == 0)
// 	{
// 		builtin_pwd(node->args, envp);
// 	}
// 	return (0);
// }

static int	get_envp_length(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**make_envp_copy(char **envp)
{
	char	**copy;
	int		length;
	int		i;

	if (!envp)
		return (NULL);
	length = get_envp_length(envp);
	copy = ft_calloc(length + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < length)
	{
		copy[i] = str_dupli(envp[i]);
		i++;
	}
	return (copy);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

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
	int	i;

	(void)argv;
	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int builtin_exit(char *argv[], char *envp[])
{
	(void)envp;
	(void)argv;
	return (0);
}
