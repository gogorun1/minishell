/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcao <lcao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:20:48 by lcao              #+#    #+#             */
/*   Updated: 2025/06/17 21:24:27 by lcao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = (char *)malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strcat(result, s2);
	ft_strcat(result, s3);
	return (result);
}

static void	handle_special_char_pipe(char *line, int *i, t_token **tokens)
{
	(void)line;
	add_token(tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
	(*i)++;
}

static void	handle_special_char_lt(char *line, int *i, t_token **tokens)
{
	if (line[*i + 1] == '<')
	{
		add_token(tokens, create_token(ft_strdup("<<"), TOKEN_HEREDOC));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, create_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
		(*i)++;
	}
}

static void	handle_special_char_gt(char *line, int *i, t_token **tokens)
{
	if (line[*i + 1] == '>')
	{
		add_token(tokens, create_token(ft_strdup(">>"), TOKEN_APPEND));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
		(*i)++;
	}
}

void	handle_special_char(char *line, int *i, t_token **tokens)
{
	if (line[*i] == '|')
		handle_special_char_pipe(line, i, tokens);
	else if (line[*i] == '<')
		handle_special_char_lt(line, i, tokens);
	else if (line[*i] == '>')
		handle_special_char_gt(line, i, tokens);
}
