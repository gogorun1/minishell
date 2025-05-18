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

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (len > n)
		len = n;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

int	ft_count(const char *s, const char *set)
{
	int	count;

	count = 0;
	if (!s || !set)
		return (0);
	while (s[count] && !ft_strchr(set, s[count]))
		count++;
	return (count);
}

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

t_token	*tokenizer(char *line)
{
	t_token	*tokens = NULL;
	char	*word;
	int		i = 0;

	while (line[i])
	{
		if (line[i] == '|')
			add_token(&tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_token(&tokens, create_token(ft_strdup("<<"), TOKEN_HEREDOC));
				i++;
			}
			else
				add_token(&tokens, create_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
		}
		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				add_token(&tokens, create_token(ft_strdup(">>"), TOKEN_APPEND));
				i++;
			}
			else
				add_token(&tokens, create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
		}
		else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
		{
			word = ft_strndup(line + i, ft_count(line + i, " \t\n"));
			if (!word)
				return (NULL);
			add_token(&tokens, create_token(word, TOKEN_WORD));
			i += strlen(word) - 1;
			free(word);
		}
		i++;
	}
	return (tokens);
}