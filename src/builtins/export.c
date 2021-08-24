#include "minishell.h"

static t_bool variable_already_exits_in_envp(char **envp, char *name)
{
	int i;
	int length;

	length = 0;
	while (name[length] && name[length] != '=')
		length++;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(name, envp[i], length) == 0 && envp[i][length] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static int modify_var_in_envp(char **envp, char *name)
{
	int length;
	int i;
	t_bool should_append_equal;

	length = 0;
	while (name[length] && name[length] != '=')
		length++;
	should_append_equal = FALSE;
	if (name[length] != '=')
		should_append_equal = TRUE;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(name, envp[i], length) == 0 && envp[i][length] == '=')
		{
			free(envp[i]);
			envp[i] = ft_calloc(str_len(name) + 2, sizeof(char));
			if (!envp[i])
				return (1);
			ft_strncpy(envp[i], name, str_len(name));
			if (should_append_equal)
				envp[i][str_len(name)] = '=';
			return (0);
		}
		i++;
	}
	return (0);
}

static int insert_new_value_in_envp(char *name)
{
	int i;

	g_shell.envp = realloc_envp_with_one_more_line();
	if (!g_shell.envp)
		return (1);
	i = 0;
	while (g_shell.envp[i])
		i++;
	g_shell.envp[i] = ft_calloc(str_len(name) + 2, sizeof(char));
	if (!g_shell.envp[i])
		return (1);
	ft_strncpy(g_shell.envp[i], name, str_len(name));
	if (!str_has(name, '='))
		g_shell.envp[i][str_len(name)] = '=';
	return (0);
}

int builtin_export(char *argv[])
{
	int i;
	t_bool has_met_an_error;

	if (!argv[1])
		return (print_sorted_envp(g_shell.envp));
	has_met_an_error = FALSE;
	i = 1;
	while (argv[i])
	{
		if (!is_a_valid_exported_name(argv[i]))
		{
			print_export_name_error(argv[i]);
			has_met_an_error = TRUE;
			i++;
			continue;
		}
		if (variable_already_exits_in_envp(g_shell.envp, argv[i]))
			modify_var_in_envp(g_shell.envp, argv[i]);
		else
			insert_new_value_in_envp(argv[i]);
		i++;
	}
	if (has_met_an_error)
		return (1);
	return (0);
}
