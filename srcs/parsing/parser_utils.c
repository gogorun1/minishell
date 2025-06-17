/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:25 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 18:58:53 by wding            ###   ########.fr       */
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
redir_type_t	get_redirection_type(t_token_type token_type)
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
ast_node_t	*init_command_node(void)
{
	ast_node_t	*node;

	node = malloc(sizeof(ast_node_t));
	if (!node)
	{
		perror("malloc error for AST node");
		return (NULL);
	}
	node->type = AST_COMMAND;
	node->data.command.args = NULL;
	node->data.command.redirs = NULL;
	return (node);
}

/* Handle word token (arguments) */
char	**handle_word_token(parser_t *parser, char **args, int *arg_count)
{
	char	**new_args;

	new_args = realloc(args, sizeof(char *) * (*arg_count + 2));
	if (!new_args)
	{
		perror("realloc error for args");
		return (NULL);
	}
	new_args[*arg_count] = strdup(parser->current->value);
	new_args[*arg_count + 1] = NULL;
	(*arg_count)++;
	parser->current = parser->current->next;
	return (new_args);
}

/* Create pipe node for pipeline */
ast_node_t	*create_pipe_node(ast_node_t *left, ast_node_t *right)
{
	ast_node_t	*pipe_node;

	pipe_node = malloc(sizeof(ast_node_t));
	if (!pipe_node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	pipe_node->type = AST_PIPE;
	pipe_node->data.binary.left = left;
	pipe_node->data.binary.right = right;
	return (pipe_node);
}
