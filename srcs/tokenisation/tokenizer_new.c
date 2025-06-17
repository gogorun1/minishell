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
