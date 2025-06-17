/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:45:50 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 15:54:02 by lcao             ###   ########.fr       */
/*                                                                            */
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

static int	tokenizer_handle_quote(const char *line, int *i, char **word, int *in_word, t_shell *g_shell)
{
	char	quote;
	int		start;
	char	*temp;
	char	*expanded;
	char	*joined;

	quote = line[(*i)++];
	start = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (!line[*i])
		return (0);
	temp = ft_strndup(line + start, *i - start);
	if (quote == '"')
	{
		expanded = expand_variables(temp, g_shell);
		free(temp);
		temp = expanded;
	}
	if (*in_word)
	{
		joined = ft_strjoin(*word, temp);
		free(*word);
		free(temp);
		*word = joined;
	}
	else
	{
		*word = temp;
		*in_word = 1;
	}
	(*i)++;
	return (1);
}

static int	tokenizer_handle_word(const char *line, int *i, char **word, int *in_word, t_shell *g_shell)
{
	int	start;
	char	*temp;
	char	*joined;
	char	*expanded;

	start = *i;
	while (line[*i] && !is_special_char(line[*i]) &&
		line[*i] != ' ' && line[*i] != '\t' &&
		line[*i] != '"' && line[*i] != '\'')
		(*i)++;
	temp = ft_strndup(line + start, *i - start);
	if (temp)
		expanded = expand_variables(temp, g_shell);
	free(temp);
	if (*in_word)
	{
		joined = ft_strjoin(*word, expanded);
		free(*word);
		free(expanded);
		*word = joined;
	}
	else
	{
		if (!*expanded)
		{
			free(expanded);
			return (0);
		}
		*word = expanded;
		*in_word = 1;
	}
	return (1);
}

t_token	*tokenizer(char *line, t_shell *g_shell)
{
	t_token	*tokens;
	int		i;
	char	*word;
	int		in_word;

	tokens = NULL;
	i = 0;
	in_word = 0;
	word = NULL;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			if (!tokenizer_handle_quote(line, &i, &word, &in_word, g_shell))
			{
				free_token_list(tokens);
				ft_fprintf(2, "Error: Quote need to be completed\n");
				return (NULL);
			}
		}
		else if (line[i] == ' ' || line[i] == '\t')
		{
			if (in_word)
			{
				add_token(&tokens, create_token(word, TOKEN_WORD));
				word = NULL;
				in_word = 0;
			}
			i++;
		}
		else if (is_special_char(line[i]))
		{
			if (in_word)
			{
				add_token(&tokens, create_token(word, TOKEN_WORD));
				word = NULL;
				in_word = 0;
			}
			handle_special_char(line, &i, &tokens);
		}
		else
			tokenizer_handle_word(line, &i, &word, &in_word, g_shell);
	}
	if (in_word)
		add_token(&tokens, create_token(word, TOKEN_WORD));
	add_token(&tokens, create_token(NULL, TOKEN_EOF));
	return (tokens);
}
