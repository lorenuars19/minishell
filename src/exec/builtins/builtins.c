#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

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

int builtin_echo(char *argv[])
{
	int		i;
	t_bool	should_print_trailing_nl;

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

int builtin_cd(char *argv[])
{
	char	*path;

	if (!argv[1])
		path = get_value_from_envp("HOME", g_info.envp);
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

int builtin_pwd(char *argv[])
{
	char	*pwd;

	(void)argv;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] == s2[i] && s1[i] && s2[i])
	{
		i++;
	}
	if (i == n)
		return ((unsigned char)s1[i - 1] - (unsigned char)s2[i - 1]);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

t_bool	variable_already_exits_in_envp(char **envp, char *name)
{
	int	i;
	int	length;

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
			return ;
		}
		i++;
	}
}

int builtin_unset(char *argv[])
{
	int	i;
	t_bool	has_met_an_error;

	if (!argv[1])
		return (0);
	i = 1;
	has_met_an_error = FALSE;
	while (argv && argv[i])
	{
		if (!is_a_valid_variable_name(argv[i]))
		{
			put_str_fd(STDERR_FILENO, "minishell: unset: '");
			put_str_fd(STDERR_FILENO, argv[i]);
			put_str_fd(STDERR_FILENO, "': not a valid identifier\n");
			has_met_an_error = TRUE;
		}
		else
			delete_variable_from_envp(argv[i], g_info.envp);
		i++;
	}
	if (has_met_an_error)
		return (1);
	return (0);
}

static void	sort_envp(char **envp)
{
	t_bool		has_a_swap_occured;
	char		*tmp;
	int			i;

	if (!envp || !envp[0])
		return ;
	has_a_swap_occured = TRUE;
	while (has_a_swap_occured)
	{
		has_a_swap_occured = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			if (strcmp(envp[i], envp[i + 1]) > 0)
			{
				has_a_swap_occured = TRUE;
				tmp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = tmp;
			}
			i++;
		}
	}
}

static void	print_one_exported_variable(char *variable)
{
	int	i;

	if (!variable)
		return ;
	if (!ft_strncmp("_=", variable, 2))
		return ;
	printf("declare -x ");
	i = 0;
	while (variable[i] && variable[i] != '=')
	{
		printf("%c", variable[i]);
		i++;
	}
	if (variable[i] == '\0')
		return ;
	printf("=\"");
	printf("%s", variable + i + 1);
	printf("\"\n");
}

static int	print_sorted_envp(char **envp)
{
	char	**dup;
	int		i;

	dup = make_envp_copy(envp);
	if (!dup)
		return (1);
	sort_envp(dup);
	i = 0;
	while (dup[i])
	{
		print_one_exported_variable(dup[i]);
		i++;
	}
	free_envp(dup);
	return (0);
}

static t_bool is_a_valid_exported_name(char *name)
{
	int i;

	if (!name)
		return (FALSE);
	if (name[0] != '_' && !is_alpha(name[0]))
		return (FALSE);
	i = 0;
	while (name[i] && name[i] != '=')
	{
		if (!is_alnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	print_export_name_error(char *name)
{
	put_str_fd(STDERR_FILENO, "minishell: export: '");
	put_str_fd(STDERR_FILENO, name);
	put_str_fd(STDERR_FILENO, "': not a valid identifier\n");
}

int	modify_var_in_envp(char **envp, char *name)
{
	int	length;
	int	i;
	t_bool	should_append_equal;

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

char **realloc_envp_with_one_more_line(void)
{
	int		new_length;
	int		i;
	char	**new_envp;

	new_length = get_envp_length(g_info.envp) + 1;
	new_envp = ft_calloc(new_length + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < new_length - 1)
	{
		new_envp[i] = g_info.envp[i];
		i++;
	}
	free(g_info.envp);
	return (new_envp);
}

int	insert_new_value_in_envp(char *name)
{
	int	i;

	g_info.envp = realloc_envp_with_one_more_line();
	if (!g_info.envp)
		return (1);
	i = 0;
	while (g_info.envp[i])
		i++;
	g_info.envp[i] = ft_calloc(str_len(name) + 2, sizeof(char));
	if (!g_info.envp[i])
		return (1);
	ft_strncpy(g_info.envp[i], name, strlen(name));
	if (!str_has(name, '='))
		g_info.envp[i][str_len(name)] = '=';
	return (0);
}

int builtin_export(char *argv[])
{
	int		i;
	t_bool	has_met_an_error;

	if (!argv[1])
		return (print_sorted_envp(g_info.envp));
	has_met_an_error = FALSE;
	i = 1;
	while (argv[i])
	{
		if (!is_a_valid_exported_name(argv[i]))
		{
			print_export_name_error(argv[i]);
			has_met_an_error = TRUE;
			i++;
			continue ;
		}
		if (variable_already_exits_in_envp(g_info.envp, argv[i]))
			modify_var_in_envp(g_info.envp, argv[i]);
		else
			insert_new_value_in_envp(argv[i]);
		i++;
	}
	if (has_met_an_error)
		return (1);
	return (0);
}

int builtin_env(char *argv[])
{
	int	i;

	(void)argv;
	i = 0;
	while (g_info.envp[i])
	{
		printf("%s\n", g_info.envp[i]);
		i++;
	}
	return (0);
}

int builtin_exit(char *argv[])
{
	int	exit_status;

	if (!argv[1])
		exit_status = 0;
	free_envp(g_info.envp);
	free_nodes(g_info.nodes);
	exit(exit_status);
}

int	builtin_empty(char*argv[])
{
	(void)argv;
	return (0);
}
