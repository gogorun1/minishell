/* ************************************************************************** */
/*                    ADDITIONS TO t_shell STRUCTURE                         */
/* ************************************************************************** */

// Add these fields to your t_shell structure in minishell.h:
typedef struct s_shell
{
	// ... your existing fields ...
	
	// Tokenizer working variables
	char		*current_line;		// Current line being tokenized
	int			line_index;			// Current position in line
	char		*current_word;		// Word being built
	int			in_word_flag;		// Flag indicating if we're building a word
	t_token		*token_list;		// Current token list being built
	
	// ... rest of your existing fields ...
} t_shell;

/* ************************************************************************** */
/*                          FILE 1: token_utils.c                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *str, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = str;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
}

void	free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free_token(temp);
	}
}

/* ************************************************************************** */
/*                          FILE 2: token_helpers.c                          */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quote_expansion(char *temp, char quote, t_shell *shell)
{
	char	*expanded;

	if (quote == '"')
	{
		expanded = expand_variables(temp, shell);
		free(temp);
		return (expanded);
	}
	return (temp);
}

char	*join_or_assign_word(char *temp, t_shell *shell)
{
	char	*joined;

	if (shell->in_word_flag)
	{
		joined = ft_strjoin(shell->current_word, temp);
		free(shell->current_word);
		free(temp);
		shell->current_word = joined;
	}
	else
	{
		shell->current_word = temp;
		shell->in_word_flag = 1;
	}
	return (shell->current_word);
}

int	check_empty_expansion(char *expanded)
{
	if (!*expanded)
	{
		free(expanded);
		return (0);
	}
	return (1);
}

void	handle_whitespace(t_shell *shell)
{
	if (shell->in_word_flag)
	{
		add_token(&shell->token_list, 
			create_token(shell->current_word, TOKEN_WORD));
		shell->current_word = NULL;
		shell->in_word_flag = 0;
	}
}

void	handle_special_token(t_shell *shell)
{
	if (shell->in_word_flag)
	{
		add_token(&shell->token_list, 
			create_token(shell->current_word, TOKEN_WORD));
		shell->current_word = NULL;
		shell->in_word_flag = 0;
	}
	handle_special_char(shell->current_line, &shell->line_index, 
		&shell->token_list);
}

/* ************************************************************************** */
/*                          FILE 3: token_quotes.c                           */
/* ************************************************************************** */

#include "minishell.h"

static int	extract_quoted_content(char quote, t_shell *shell)
{
	int		start;
	char	*temp;

	start = shell->line_index;
	while (shell->current_line[shell->line_index] 
		&& shell->current_line[shell->line_index] != quote)
		shell->line_index++;
	if (!shell->current_line[shell->line_index])
		return (0);
	temp = ft_strndup(shell->current_line + start, 
		shell->line_index - start);
	temp = handle_quote_expansion(temp, quote, shell);
	join_or_assign_word(temp, shell);
	shell->line_index++;
	return (1);
}

int	tokenizer_handle_quote(t_shell *shell)
{
	char	quote;

	quote = shell->current_line[shell->line_index++];
	return (extract_quoted_content(quote, shell));
}

static char	*extract_word_content(t_shell *shell)
{
	int		start;
	char	*temp;
	char	*expanded;

	start = shell->line_index;
	while (shell->current_line[shell->line_index] 
		&& !is_special_char(shell->current_line[shell->line_index])
		&& shell->current_line[shell->line_index] != ' ' 
		&& shell->current_line[shell->line_index] != '\t'
		&& shell->current_line[shell->line_index] != '"' 
		&& shell->current_line[shell->line_index] != '\'')
		shell->line_index++;
	temp = ft_strndup(shell->current_line + start, 
		shell->line_index - start);
	if (temp)
		expanded = expand_variables(temp, shell);
	free(temp);
	return (expanded);
}

int	tokenizer_handle_word(t_shell *shell)
{
	char	*expanded;

	expanded = extract_word_content(shell);
	if (shell->in_word_flag)
		join_or_assign_word(expanded, shell);
	else
	{
		if (!check_empty_expansion(expanded))
			return (0);
		shell->current_word = expanded;
		shell->in_word_flag = 1;
	}
	return (1);
}

t_token	*handle_quote_error(t_shell *shell)
{
	free_token_list(shell->token_list);
	ft_fprintf(2, "Error: Quote need to be completed\n");
	return (NULL);
}

/* ************************************************************************** */
/*                          FILE 4: tokenizer.c                              */
/* ************************************************************************** */

#include "minishell.h"

static void	init_tokenizer_state(char *line, t_shell *shell)
{
	shell->current_line = line;
	shell->line_index = 0;
	shell->current_word = NULL;
	shell->in_word_flag = 0;
	shell->token_list = NULL;
}

static void	process_quote_char(t_shell *shell)
{
	if (!tokenizer_handle_quote(shell))
	{
		shell->token_list = handle_quote_error(shell);
		return ;
	}
}

static void	process_whitespace_char(t_shell *shell)
{
	handle_whitespace(shell);
	shell->line_index++;
}

static void	finalize_tokenizer(t_shell *shell)
{
	if (shell->in_word_flag && shell->token_list)
		add_token(&shell->token_list, 
			create_token(shell->current_word, TOKEN_WORD));
	if (shell->token_list)
		add_token(&shell->token_list, create_token(NULL, TOKEN_EOF));
}

t_token	*tokenizer(char *line, t_shell *shell)
{
	init_tokenizer_state(line, shell);
	while (shell->current_line[shell->line_index] && shell->token_list)
	{
		if (shell->current_line[shell->line_index] == '"' 
			|| shell->current_line[shell->line_index] == '\'')
			process_quote_char(shell);
		else if (shell->current_line[shell->line_index] == ' ' 
			|| shell->current_line[shell->line_index] == '\t')
			process_whitespace_char(shell);
		else if (is_special_char(shell->current_line[shell->line_index]))
			handle_special_token(shell);
		else
			tokenizer_handle_word(shell);
	}
	finalize_tokenizer(shell);
	return (shell->token_list);
}

/* ************************************************************************** */
/*                       UPDATED minishell.h ADDITIONS                       */
/* ************************************************************************** */

// REPLACE your existing t_shell structure with this updated version:
typedef struct s_shell
{
	// ... your existing fields ...
	
	// Tokenizer working variables (ADD THESE)
	char		*current_line;		// Current line being tokenized
	int			line_index;			// Current position in line
	char		*current_word;		// Word being built
	int			in_word_flag;		// Flag indicating if we're building a word
	t_token		*token_list;		// Current token list being built
	
	// ... rest of your existing fields ...
} t_shell;

// REPLACE your function prototypes with these updated ones:

// Token utilities (token_utils.c) - NO CHANGES
t_token		*create_token(char *str, t_token_type type);
void		add_token(t_token **head, t_token *new_token);
void		free_token(t_token *token);
void		free_token_list(t_token *head);

// Token helpers (token_helpers.c) - UPDATED SIGNATURES
char		*handle_quote_expansion(char *temp, char quote, t_shell *shell);
char		*join_or_assign_word(char *temp, t_shell *shell);
int			check_empty_expansion(char *expanded);
void		handle_whitespace(t_shell *shell);
void		handle_special_token(t_shell *shell);

// Token quotes handling (token_quotes.c) - UPDATED SIGNATURES
int			tokenizer_handle_quote(t_shell *shell);
int			tokenizer_handle_word(t_shell *shell);
t_token		*handle_quote_error(t_shell *shell);

// Main tokenizer (tokenizer.c) - UPDATED SIGNATURE
t_token		*tokenizer(char *line, t_shell *shell);

/* ************************************************************************** */
/*                              USAGE NOTES                                  */
/* ************************************************************************** */

// HOW TO USE THE NEW TOKENIZER:
// Before: tokens = tokenizer(line, shell);
// After:  tokens = tokenizer(line, shell);  // Same call!
//
// The tokenizer now uses shell's internal state, making it cleaner
// and compliant with the 4-argument limit.
//
// IMPORTANT: The shell structure now holds tokenizer state, so make sure
// to initialize your shell structure properly before calling tokenizer.

/* ************************************************************************** */
/*                              MAKEFILE - NO CHANGES                        */
/* ************************************************************************** */

// Your Makefile remains the same:
// SRCS = token_utils.c \
//        token_helpers.c \
//        token_quotes.c \
//        tokenizer.c \
//        # ... your other source files