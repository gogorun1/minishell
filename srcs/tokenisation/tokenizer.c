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

// char	*ft_strndup(const char *s, size_t n)
// {
// 	char	*dup;
// 	size_t	len;

// 	len = ft_strlen(s);
// 	if (len > n)
// 		len = n;
// 	dup = (char *)malloc(len + 1);
// 	if (!dup)
// 		return (NULL);
// 	ft_strlcpy(dup, s, len + 1);
// 	return (dup);
// }

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

// 处理引号内的内容，区分单双引号的行为
// int handle_quotes(char **input, int *i, t_token **tokens)
// {
//     char quote = (*input)[*i];
//     int start = *i + 1;
//     int j = start;
    
//     // 寻找匹配的引号
//     while ((*input)[j] && (*input)[j] != quote)
//         j++;
        
//     if (!(*input)[j]) // 未闭合引号处理
//     {
//         ft_printf("minishell: unexpected EOF while looking for matching %c\n", quote);
//         return 0; // 简化版本
//     }
    
//     // 提取引号内容
//     char *content = ft_strndup(*input + start, j - start);
    
//     // 根据引号类型决定是否进行变量展开
//     if (quote == '"')
//     {
//         // 双引号内进行变量展开
//         char *expanded = expand_variables(content);
//         free(content);
//         content = expanded;
//     }
//     // 单引号内保持字面值，不做任何处理
    
//     // 创建token并添加到链表
//     add_token(tokens, create_token(content, 0));
    
//     *i = j + 1; // 更新索引到引号后面
//     return 1;
// }

// 变量展开函数
char *expand_variables(char *str)
{
    char *result = ft_strdup("");
    char *temp;
    int i = 0;
    int start = 0;
    
    while (str[i])
    {
        if (str[i] == '$' && is_valid_var_char(str[i+1]))
        {
            // 添加$前面的内容
            if (i > start)
            {
                char *part = ft_strndup(str + start, i - start);
                temp = ft_strjoin(result, part);
                free(result);
                free(part);
                result = temp;
            }
            
            // 处理变量名
            int var_start = i + 1;
            int var_len = 0;
            
            while (str[var_start + var_len] && is_valid_var_char(str[var_start + var_len]))
                var_len++;
                
            char *var_name = ft_strndup(str + var_start, var_len);
            char *var_value = get_env_value(var_name);
            
            if (var_value)
            {
                temp = ft_strjoin(result, var_value);
                free(result);
                result = temp;
            }
            
            free(var_name);
            i = var_start + var_len;
            start = i;
        }
        else
            i++;
    }
    
    // 添加剩余部分
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
				char *expanded = expand_variables(temp);
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
        // 处理变量 暂时有segfault
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
			printf("Extracted variable name: %s, shell envronment list: %s\n", var_name, g_shell->env_list ? "exists" : "NULL");
            char *var_value = my_getenv(var_name, g_shell->env_list); // 使用自定义getenv
			printf("Variable name: %s, Value: %s\n", var_name, var_value ? var_value : "NULL");
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
    
    // 处理最后一个单词
    if (in_word)
	{
        add_token(&tokens, create_token(word, TOKEN_WORD));
	}
	add_token(&tokens, create_token(NULL, TOKEN_EOF)); // 添加EOF标记
    return tokens;
}


// t_token	*tokenizer(char *line)
// {
// 	t_token	*tokens = NULL;
// 	char	*word;
// 	int		i = 0;
// 	int		word_start = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '|')
// 			add_token(&tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
// 		else if (line[i] == '<')
// 		{
// 			if (line[i + 1] == '<')
// 			{
// 				add_token(&tokens, create_token(ft_strdup("<<"), TOKEN_HEREDOC));
// 				i++;
// 			}
// 			else
// 				add_token(&tokens, create_token(ft_strdup("<"), TOKEN_REDIRECT_IN));
// 		}
// 		else if (line[i] == '>')
// 		{
// 			if (line[i + 1] == '>')
// 			{
// 				add_token(&tokens, create_token(ft_strdup(">>"), TOKEN_APPEND));
// 				i++;
// 			}
// 			else
// 				add_token(&tokens, create_token(ft_strdup(">"), TOKEN_REDIRECT_OUT));
// 		}
// 		else if (line[i] == '\'' || line[i] == '"')
//         {
//             char quote = line[i];
//             t_token_type type = (quote == '"') ? TOKEN_DQUOTE : TOKEN_SQUOTE;
            
//             word_start = i + 1;  // 跳过开始的引号
//             i++;
            
//             // 寻找匹配的引号
//             while (line[i] && line[i] != quote)
//                 i++;
                
//             if (!line[i])  // 未闭合的引号
//             {
//                 char *buffer = ft_strndup(line + word_start, i - word_start);
//                 char *temp;
//                 bool quote_closed = false;
                
//                 // 循环读取新行，直到找到闭合引号
//                 while (!quote_closed)
//                 {
//                     // 添加换行符
//                     temp = ft_strjoin(buffer, "\n");
//                     free(buffer);
//                     buffer = temp;
                    
//                     // 读取新行
//                     char *new_line = readline("> ");
//                     if (!new_line)  // 处理EOF
//                     {
//                         free(buffer);
//                         fprintf(stderr, "minishell: unexpected EOF while looking for matching `%c'\n", quote);
//                         return (NULL);
//                     }
                    
//                     // 在新行中查找闭合引号
//                     int j = 0;
//                     while (new_line[j] && new_line[j] != quote)
//                         j++;
                        
//                     // 添加新行内容到buffer
//                     temp = ft_strjoin(buffer, ft_strndup(new_line, j));
//                     free(buffer);
//                     buffer = temp;
                    
//                     // 如果找到闭合引号
//                     if (new_line[j] == quote)
//                     {
//                         quote_closed = true;
                        
//                         // 如果引号后还有内容，需要处理
//                         if (new_line[j + 1])
//                         {
//                             char *remaining = ft_strdup(new_line + j + 1);
//                             // char *new_input = ft_strjoin(remaining, "");
//                             // 这里需要处理剩余内容，可能parsing的时候需要递归调用tokenizer
//                             free(remaining);
//                         }
//                     }
                    
//                     free(new_line);
//                 }
                
//                 add_token(&tokens, create_token(buffer, type));
//                 break;  // 退出主循环，因为已经处理完所有输入
//             }
//             else  // 找到闭合引号
//             {
//                 word = ft_strndup(line + word_start, i - word_start);
//                 if (!word)
//                     return (NULL);
//                 add_token(&tokens, create_token(word, type));
//                 i++;  // 跳过闭合引号
//             }
//         }
// 		else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
// 		{
// 			word_start = i;
// 			while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
// 				&& line[i] != '|' && line[i] != '<' && line[i] != '>')
// 				i++;
// 			word = ft_strndup(line + word_start, i - word_start);
// 			if (!word)
// 				return (NULL);
// 			add_token(&tokens, create_token(word, TOKEN_WORD));
// 		}
// 		i++;
// 	}
// 	return (tokens);
// }