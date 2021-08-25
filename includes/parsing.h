/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 13:49:51 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 14:13:54 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define MALLOC_ERROR -1
# define SPECIAL_CHARS "|\'\" <>\t"

typedef enum e_token_type
{
	T_GENERAL = 'g',
	T_SPACE = ' ',
	T_TAB = '\t',
	T_BLANK = 'b',
	T_PIPE = '|',
	T_SQUOTE = '\'',
	T_DQUOTE = '\"',
	T_SMALLER = '<',
	T_GREATER = '>',
	T_HEREDOC = 'h',
	T_APPEND = 'a'
}		t_token_type;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef enum e_redirection_mode
{
	M_INPUT = T_SMALLER,
	M_TRUNCATE = T_GREATER,
	M_HEREDOC = T_HEREDOC,
	M_APPEND = T_APPEND
}	t_redirection_mode;

typedef struct s_redirection
{
	char					*filename;
	t_redirection_mode		mode;
	t_bool					should_expand;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_node	t_node;

typedef enum e_node_type
{
	COMMAND_NODE,
	PIPE_NODE
}	t_node_type;

struct s_node
{
	t_node_type		type;
	char			**args;
	t_redirection	*redirections;
	t_node			*right;
	t_node			*left;
	pid_t			pid;
};

int				parse(char *line);
int				syntax_checker(char *line, t_token *tokens);
t_token			*scanner(char *line);
int				expand_variables(char **envp, t_token *tokens);
int				merge_tokens(t_token *tokens);
t_node			*parser(t_token *tokens);

t_token_type	get_token_type(char c);
void			print_tokens(t_token *tokens);
char			*ft_strcdup(char *str, char c);
char			*ft_strdup_set(char *str, char *set);
void			free_tokens_excl_data(t_token *tokens);
void			free_tokens_incl_data(t_token *tokens);
void			*ft_calloc(size_t nmemb, size_t size);
void			print_nodes(t_node *nodes, int spaces);
void			free_nodes(t_node *nodes);
int				ft_strncpy(char *dest, char *src, int n);
t_bool			has_redirection_type(t_token *token);
void			skip_blank_tokens(t_token **tokens);
char			**make_envp_copy(char **envp);
void			free_envp(char **envp);
char			*get_value_from_envp(char *name, char **envp);
int				expand_in_one_token(t_token *token, char **envp);
t_bool			contains_redirections(t_token *tokens);
int				get_redirections(t_token *tokens, t_node *node);
t_bool			is_pipe_next(t_token *tokens);
char			**get_args(t_token *tokens);
void			skip_tokens_until_next_command(t_token **tokens);
int				get_quote_token(char *line, t_token *token, int *index);
int				get_general_token(char *line, t_token *token, int *index);
int				get_blank_token(char *line, t_token *token, int *index);
int				get_pipe_token(char *line, t_token *token, int *index);
int				get_redirection_token(char *line, t_token *token, int *index);
t_bool			token_contains_a_string(t_token *token);
t_bool			line_contains_only_blanks(char *line);
char			*get_variable_name(t_token *token);
t_bool			should_token_be_expanded(t_token *token);

#endif
