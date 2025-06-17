/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:25 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 23:12:46 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if current token is a redirection */
int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

/* Get redirection type from token */
t_redir_type	get_redirection_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN)
		return (REDIR_IN);
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUT);
	else if (token_type == TOKEN_APPEND)
		return (REDIR_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (-1);
}

/* Initialize command node */
t_ast_node	*init_command_node(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		perror("malloc error for AST node");
		return (NULL);
	}
	node->type = AST_COMMAND;
	node->u_data.command.args = NULL;
	node->u_data.command.redirs = NULL;
	return (node);
}

/* Handle word token (arguments) */
char	**handle_word_token(t_parser *parser, char **args, int *arg_count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (*arg_count + 2));
	if (!new_args)
	{
		perror("malloc error for args");
		return (NULL);
	}
	i = 0;
	while (i < *arg_count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[*arg_count] = ft_strdup(parser->current->value);
	new_args[*arg_count + 1] = NULL;
	(*arg_count)++;
	if (args)
		free(args);
	parser->current = parser->current->next;
	return (new_args);
}

/* Create pipe node for pipeline */
t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	if (!pipe_node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	pipe_node->type = AST_PIPE;
	pipe_node->u_data.s_binary.left = left;
	pipe_node->u_data.s_binary.right = right;
	return (pipe_node);
}
