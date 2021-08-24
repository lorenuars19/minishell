#include "minishell.h"
#include <limits.h>

t_bool	is_a_valid_arg(char *str)
{
	int		i;
	t_bool	was_digit_found;

	was_digit_found = FALSE;
	i = 0;
	while (is_wsp(str[i]) == 1)
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		i++;
		was_digit_found = TRUE;
	}
	while (is_wsp(str[i]) == 1)
		i++;
	if (str[i] != '\0' || was_digit_found == FALSE)
		return (FALSE);
	return (TRUE);
}

int	atoi_exit(char *str)
{
	int					sign;
	unsigned long long	num;
	int					i;

	if (!is_a_valid_arg(str))
		return (-1);
	i = 0;
	num = 0;
	sign = 1;
	while (is_wsp(str[i]) == 1)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + str[i++] - '0';
		if (sign == 1 && num > (unsigned long long)LONG_MAX)
			return (-1);
		else if (sign == -1 && num > (unsigned long long)LONG_MAX + 1)
			return (-1);
	}
	return ((unsigned char)(sign * num));
}

int	print_exit_error(char *arg)
{
	put_str_fd(STDERR_FILENO, "exit\nminishell: exit: ");
	put_str_fd(STDERR_FILENO, arg);
	put_str_fd(STDERR_FILENO, ": numeric argument required\n");
	return (2);
}

int	print_argc_error(void)
{
	put_str_fd(STDERR_FILENO, "exit\nminishell: exit: too many arguments\n");
	return (1);
}

int	builtin_exit(char *argv[])
{
	int	exit_status;
	int	i;

	if (argv[1] && argv[2])
		return (print_argc_error());
	i = 1;
	while (argv[i])
	{
		exit_status = atoi_exit(argv[i]);
		if (exit_status == -1)
		{
			print_exit_error(argv[i]);
			free_envp(g_shell.envp);
			free_nodes(g_shell.nodes);
			exit(2);
		}
		i++;
	}
	free_envp(g_shell.envp);
	free_nodes(g_shell.nodes);
	printf("exit\n");
	if (i == 1)
		exit(0);
	exit(exit_status);
}
