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
// char *expand_variables(char *str, t_shell *g_shell)
// {
// 	char *result;
// 	char *temp;
// 	char *var_value;
// 	int var_len;
// 	int var_start;
// 	int i = 0;
// 	int start = 0;

// 	result = ft_strdup("");
// 	while (str[i])
// 	{
// 		if (str[i] == '$' && is_valid_var_char(str[i + 1]))
// 		{
// 			if (i > start)
// 				temp = ft_strjoin(result, ft_strndup(str + start, i - start));
// 			free(result);
// 			var_start = i + 1;
// 			var_len = ft_var_name_len(str + var_start);
// 			var_value = get_variable_value(str + var_start, var_len, g_shell);
// 			if (var_value)
// 			{
// 				result = ft_strjoin(temp, var_value);
// 				free(temp);
// 			}
// 			free(var_value);
// 			i = var_start + var_len;
// 			start = i;
// 		}
// 		else
// 			i++;
// 	}
// 	if (str[start])
// 	{
// 		temp = ft_strjoin(result, str + start);
// 		free(result);
// 		result = temp;
// 	}
// 	return result;
// }


// Expands environment variables in the string
char *expand_variables(char *str, t_shell *g_shell)
{
	char *result;
	char *temp;
	int i = 0;
	int start = 0;

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
			int var_start = i + 1;
			int var_len = 0;
			while (str[var_start + var_len] && is_valid_var_char(str[var_start + var_len]))
				var_len++;
			char *var_value = get_variable_value(str + var_start, var_len, g_shell);
			if (var_value)
			{
				temp = ft_strjoin(result, var_value);
				free(result);
				result = temp;
			}
			i = var_start + var_len;
			start = i;
		}
		else
			i++;
	}
	if (str[start])
	{
		temp = ft_strjoin(result, str + start);
		free(result);
		result = temp;
	}
	return result;
}

t_token *tokenizer(char *line, t_shell *g_shell)
{
    t_token *tokens = NULL;
    int i = 0;
    int start;
    char *word = NULL;
    char *temp;
    int in_word = 0;
    
    while (line[i])
    {
        // 处理引号
        if (line[i] == '"' || line[i] == '\'')
        {
            char quote = line[i++];
            start = i;
            
            // 寻找匹配的引号
            while (line[i] && line[i] != quote)
                i++;
                
            if (!line[i]) // 未闭合引号处理
            {
                return NULL; // 简化版本，实际应该处理多行输入
            }
            // 提取引号内容
            temp = ft_strndup(line + start, i - start);
            
			if (quote == '"')
			{
				// 双引号内进行变量展开
				char *expanded = expand_variables(temp, g_shell);
				free(temp);
				temp = expanded;
			}
            // 如果已经在收集单词，则连接；否则创建新单词
            if (in_word)
            {
                char *joined = ft_strjoin(word, temp);
                free(word);
                free(temp);
                word = joined;
            }
            else
            {
                word = temp;
                in_word = 1;
            }
            
            i++; // 跳过闭合引号

        }
        else if (line[i] == '$' && is_valid_var_char(line[i + 1]))
        {
			printf("Variable found at index %d: %s\n", i, line + i);

			// 处理变量展开
            start = i;
            i++; // 跳过$
            
            // 收集变量名
            while (line[i] && is_valid_var_char(line[i]))
                i++;
                
            // 提取变量名并获取值
            char *var_name = ft_strndup(line + start + 1, i - start - 1);
            char *var_value = my_getenv(var_name, g_shell->env_list); 
            free(var_name);
            
            // 连接到当前单词
            if (in_word)
            {
                char *joined = ft_strjoin(word, var_value ? var_value : "");
                free(word);
                free(var_value);
                word = joined;
            }
            else
            {
                word = var_value ? var_value : ft_strdup("");
                in_word = 1;
            }
        }
        // 处理空白字符
        else if (line[i] == ' ' || line[i] == '\t')
        {
            // 如果正在收集单词，则结束当前单词
            if (in_word)
            {
                add_token(&tokens, create_token(word, TOKEN_WORD));
                word = NULL;
                in_word = 0;
            }
            i++;
        }
        // 处理特殊字符
        else if (is_special_char(line[i]))
        {
            // 如果正在收集单词，则结束当前单词
            if (in_word)
            {
                add_token(&tokens, create_token(word, TOKEN_WORD));
                word = NULL;
                in_word = 0;
            }
            
            // 处理特殊字符...
            handle_special_char(line, &i, &tokens);
        }
        // 处理普通字符
        else
        {
            start = i;
            while (line[i] && !is_special_char(line[i]) && 
                  line[i] != ' ' && line[i] != '\t' && 
                  line[i] != '"' && line[i] != '\'' && 
                  line[i] != '$')
                i++;
            temp = ft_strndup(line + start, i - start);
            // 连接到当前单词或创建新单词
            if (in_word)
            {
                char *joined = ft_strjoin(word, temp);
                free(word);
                free(temp);
                word = joined;
            }
            else
            {
                word = temp;
                in_word = 1;
            }
        }
    }
    if (in_word)
		add_token(&tokens, create_token(word, TOKEN_WORD));
	add_token(&tokens, create_token(NULL, TOKEN_EOF));
    return tokens;
}
