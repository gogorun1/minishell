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