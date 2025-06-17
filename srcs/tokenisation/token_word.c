/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:54:02 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 22:03:36 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_word_content(const char *line, int *i)
{
	int		start;
	char	*temp;

	start = *i;
	while (line[*i] && !is_special_char(line[*i]) && line[*i] != ' '
		&& line[*i] != '\t' && line[*i] != '"' && line[*i] != '\'')
		(*i)++;
	temp = ft_strndup(line + start, *i - start);
	return (temp);
}

static char	*expand_word_content(char *temp, t_shell *shell)
{
	char	*expanded;

	if (temp)
	{
		expanded = expand_variables(temp, shell);
		free(temp);
		return (expanded);
	}
	return (NULL);
}

static int	handle_empty_expansion(char *expanded)
{
	if (!*expanded)
	{
		free(expanded);
		return (0);
	}
	return (1);
}

static void	join_or_set_word(char **word, char *expanded, int *in_word)
{
	char	*joined;

	if (*in_word)
	{
		joined = ft_strjoin(*word, expanded);
		free(*word);
		free(expanded);
		*word = joined;
	}
	else
	{
		*word = expanded;
		*in_word = 1;
	}
}

int	tokenizer_handle_word(const char *line, int *i, t_token_data *data)
{
	char	*temp;
	char	*expanded;

	temp = extract_word_content(line, i);
	expanded = expand_word_content(temp, data->shell);
	if (!expanded)
		return (0);
	if (data->in_word || handle_empty_expansion(expanded))
	{
		join_or_set_word(&data->word, expanded, &data->in_word);
		return (1);
	}
	return (0);
}
