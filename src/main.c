#include "minishell.h"
#include <stdlib.h>

static int set_prompt(t_sd *sd)
{
	char *prompt;

	prompt = NULL;
	prompt = getcwd(NULL, 0);
	prompt = str_jointo("\033[34m", prompt, &prompt);
	if (sd->status)
		prompt = str_jointo(prompt, " \033[31;1m$\033[0m ", &prompt);
	else
		prompt = str_jointo(prompt, " \033[32;1m$\033[0m ", &prompt);
	sd->line = readline(prompt);
	free(prompt);
	if (!sd->line)
	{
		printf("\nexit\n");
		free(sd->line);
		return (1);
	}
	return (0);
}

static int process_command(t_sd *sd)
{
	if (sd->line && *(sd->line))
		add_history(sd->line);
	sd->tokens = scanner(sd->line);
	if (syntax_checker(sd->line, sd->tokens) != 0)
	{
		free_tokens_with_data(sd->tokens);
		free(sd->line);
		return (1);
	}
	expand_variables(*(sd->envp_addr), sd->tokens);
	merge_tokens(sd->tokens);
	print_tokens(sd->tokens);
	sd->nodes = parser(sd->tokens);
	print_nodes(sd->nodes, 0);

	sd->status = execution(sd->nodes, *(sd->envp_addr));

	free_tokens_without_data(sd->tokens);
	free_nodes(sd->nodes);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_sd sd;
	(void)argc;
	(void)argv;

	sd = (t_sd){&envp, NULL, 0, NULL, NULL};

	sd.status = 0;

	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_sys_put("setup_signals"));

	while (1)
	{
		if (set_prompt(&sd))
			return (EXIT_SUCCESS);
//TODO remove debug
dprintf(2, "\nline : \"%s\"\n", sd.line);
		if (process_command(&sd))
			continue ;
//TODO remove debug
dprintf(2, "\n> Last command status : %d <\n", sd.status);
		if (str_cmp("exit", sd.line) == 0)
		{
			free(sd.line);
			return (EXIT_SUCCESS);
		}
		free(sd.line);
	}
	return (0);
}
