/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:54:02 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 19:23:35 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_token_data(t_token_data *data, t_shell *shell)
{
	data->shell = shell;
	data->word = NULL;
	data->in_word = 0;
}

static int	handle_quote_char(char *line, int *i, t_token_data *data)
{
	if (!tokenizer_handle_quote(line, i, data))
	{
		ft_fprintf(2, "Error: Quote need to be completed\n");
		return (0);
	}
	return (1);
}

static void	handle_whitespace(t_token **tokens, t_token_data *data, int *i)
{
	if (data->in_word)
	{
		add_token(tokens, create_token(data->word, TOKEN_WORD));
		data->word = NULL;
		data->in_word = 0;
	}
	(*i)++;
}

static void	handle_special(char *line, int *i, t_token **tokens, t_token_data *data)
{
	if (data->in_word)
	{
		add_token(tokens, create_token(data->word, TOKEN_WORD));
		data->word = NULL;
		data->in_word = 0;
	}
	handle_special_char(line, i, tokens);
}

t_token	*tokenizer(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_token_data	data;
	int			i;

	tokens = NULL;
	i = 0;
	init_token_data(&data, shell);
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			if (!handle_quote_char(line, &i, &data))
				return (free_token_list(tokens), NULL);
		}
		else if (line[i] == ' ' || line[i] == '\t')
			handle_whitespace(&tokens, &data, &i);
		else if (is_special_char(line[i]))
			handle_special(line, &i, &tokens, &data);
		else
			tokenizer_handle_word(line, &i, &data);
	}
	if (data.in_word)
		add_token(&tokens, create_token(data.word, TOKEN_WORD));
	add_token(&tokens, create_token(NULL, TOKEN_EOF));
	return (tokens);
}