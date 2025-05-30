/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-18 15:45:50 by wding             #+#    #+#             */
/*   Updated: 2025-05-18 15:45:50 by wding            ###   ########.fr       */
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

static int	expand_var(const char *str, int i, char **result, t_shell *g_shell)
{
	int	var_start;
	int	var_len;
	char	*var_value;
	char	*temp;

	var_start = i + 1;
	var_len = 0;
	while (str[var_start + var_len] && is_valid_var_char(str[var_start + var_len]))
		var_len++;
	var_value = get_variable_value(str + var_start, var_len, g_shell);
	if (var_value)
	{
		temp = ft_strjoin(*result, var_value);
		free(*result);
		*result = temp;
	}
	free(var_value);
	return (var_start + var_len);
}

char	*expand_variables(char *str, t_shell *g_shell)
{
	char	*result;
	char	*temp;
	int		i;
	int		start;

	i = 0;
	start = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && is_valid_var_char(str[i + 1]))
		{
			if (i > start)
			{
				temp = ft_strjoin(result, ft_strndup(str + start, i - start));
				free(result);
				result = temp;
			}
			i = expand_var(str, i, &result, g_shell);
			start = i;
		}
		else
			i++;
	}
	if (!str[start])
		return (result);
	else return (ft_strjoin(result, str + start));
}

static int	tokenizer_handle_quote(const char *line, int *i, char **word, int *in_word, t_shell *g_shell)
{
	char	quote;
	int	start;
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

static int	tokenizer_handle_var(const char *line, int *i, char **word, int *in_word, t_shell *g_shell)
{
	int	start;
	char	*var_name;
	char	*var_value;
	char	*joined;

	start = *i;
	(*i)++;
	while (line[*i] && is_valid_var_char(line[*i]))
		(*i)++;
	var_name = ft_strndup(line + start + 1, *i - start - 1);
	var_value = my_getenv(var_name, g_shell->env_list);
	free(var_name);
	if (*in_word)
	{
		joined = ft_strjoin(*word, var_value ? var_value : "");
		free(*word);
		free(var_value);
		*word = joined;
	}
	else
	{
		*word = var_value ? var_value : ft_strdup("");
		*in_word = 1;
	}
	return (1);
}

static int	tokenizer_handle_word(const char *line, int *i, char **word, int *in_word)
{
	int	start;
	char	*temp;
	char	*joined;

	start = *i;
	while (line[*i] && !is_special_char(line[*i]) &&
		line[*i] != ' ' && line[*i] != '\t' &&
		line[*i] != '"' && line[*i] != '\'' &&
		line[*i] != '$')
		(*i)++;
	temp = ft_strndup(line + start, *i - start);
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
				return (NULL);
		}
		else if (line[i] == '$' && is_valid_var_char(line[i + 1]))
			tokenizer_handle_var(line, &i, &word, &in_word, g_shell);
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
			tokenizer_handle_word(line, &i, &word, &in_word);
	}
	if (in_word)
		add_token(&tokens, create_token(word, TOKEN_WORD));
	add_token(&tokens, create_token(NULL, TOKEN_EOF));
	return (tokens);
}
