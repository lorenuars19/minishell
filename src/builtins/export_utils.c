#include "minishell.h"

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

static void sort_envp(char **envp)
{
	t_bool has_a_swap_occured;
	char *tmp;
	int i;

	if (!envp || !envp[0])
		return;
	has_a_swap_occured = TRUE;
	while (has_a_swap_occured)
	{
		has_a_swap_occured = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			if (str_cmp(envp[i], envp[i + 1]) > 0)
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

static void print_one_exported_variable(char *variable)
{
	int i;

	if (!variable)
		return;
	if (!ft_strncmp("_=", variable, 2))
		return;
	printf("declare -x ");
	i = 0;
	while (variable[i] && variable[i] != '=')
	{
		printf("%c", variable[i]);
		i++;
	}
	if (variable[i] == '\0')
		return;
	printf("=\"");
	printf("%s", variable + i + 1);
	printf("\"\n");
}

int print_sorted_envp(char **envp)
{
	char **dup;
	int i;

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

t_bool is_a_valid_exported_name(char *name)
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

void print_export_name_error(char *name)
{
	put_str_fd(STDERR_FILENO, "minishell: export: '");
	put_str_fd(STDERR_FILENO, name);
	put_str_fd(STDERR_FILENO, "': not a valid identifier\n");
}
