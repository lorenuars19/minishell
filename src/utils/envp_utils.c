#include "minishell.h"

char *get_value_from_envp(char *name, char **envp)
{
	int i;
	size_t length;

	i = 0;
	length = str_len(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, length) == 0 && envp[i][length] == '=')
			return (str_dupli(&(envp[i][length + 1])));
		i++;
	}
	return (str_dupli(""));
}

int	get_envp_length(char **envp)
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
		if (!copy[i])
			return (copy);
		i++;
	}
	return (copy);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**realloc_envp_with_one_more_line(void)
{
	int		new_length;
	int		i;
	char	**new_envp;

	new_length = get_envp_length(g_shell.envp) + 1;
	new_envp = ft_calloc(new_length + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < new_length - 1)
	{
		new_envp[i] = g_shell.envp[i];
		i++;
	}
	free(g_shell.envp);
	return (new_envp);
}
