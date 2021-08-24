#include "minishell.h"

static t_bool	is_a_valid_variable_name(char *name)
{
	int	i;

	if (!name)
		return (FALSE);
	if (name[0] != '_' && !is_alpha(name[0]))
		return (FALSE);
	i = 0;
	while (name[i])
	{
		if (!is_alnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	shift_envp_up(char **envp, int i)
{
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
}

static void	delete_variable_from_envp(char *name, char **envp)
{
	int	i;
	int	length;

	i = 0;
	length = str_len(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(name, envp[i], length) == 0 && envp[i][length] == '=')
		{
			free(envp[i]);
			shift_envp_up(envp, i);
			return;
		}
		i++;
	}
}

int	builtin_unset(char *argv[])
{
	int		i;
	t_bool	has_met_an_error;

	if (!argv[1])
		return (0);
	i = 1;
	has_met_an_error = FALSE;
	while (argv[i])
	{
		if (!is_a_valid_variable_name(argv[i]))
		{
			put_str_fd(STDERR_FILENO, "minishell: unset: '");
			put_str_fd(STDERR_FILENO, argv[i]);
			put_str_fd(STDERR_FILENO, "': not a valid identifier\n");
			has_met_an_error = TRUE;
		}
		else
			delete_variable_from_envp(argv[i], g_shell.envp);
		i++;
	}
	if (has_met_an_error)
		return (1);
	return (0);
}
