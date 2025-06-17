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

	expanded = NULL;
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
	printf("called quote error\n");
	free_token_list(shell->token_list);
	ft_fprintf(2, "Error: Quote need to be completed\n");
	return (NULL);
}