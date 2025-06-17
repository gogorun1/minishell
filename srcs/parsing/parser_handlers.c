/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:36 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 21:21:37 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle redirection parsing - READS HEREDOC CONTENT IMMEDIATELY */
int	handle_redirection(parser_t *parser, ast_node_t *node)
{
	redir_type_t	type;

	type = get_redirection_type(parser->current->type);
	if ((int)type == -1)
		return (-1);
	parser->current = parser->current->next;
	if (!parser->current || parser->current->type != TOKEN_WORD)
	{
		ft_fprintf(2, "Parse error: Expected filename after redirection\n");
		return (-1);
	}
	add_redirection(&(node->data.command), type, parser->current->value);
	if (process_heredoc_redirection(node, type) == -1)
		return (-1);
	parser->current = parser->current->next;
	return (0);
}

/* Handle word token in parse command */
int	handle_word_in_parse(parser_t *parser, char ***args, int *arg_count)
{
	*args = handle_word_token(parser, *args, arg_count);
	if (!*args)
		return (-1);
	return (0);
}

/* Handle redirection in parse command */
int	handle_redir_in_parse(parser_t *parser, ast_node_t *node)
{
	if (handle_redirection(parser, node) == -1)
		return (-1);
	return (0);
}

/* Process tokens in parse command loop */
int	process_command_tokens(parser_t *parser, ast_node_t *node, char ***args,
		int *arg_count)
{
	if (parser->current->type == TOKEN_WORD)
	{
		if (handle_word_in_parse(parser, args, arg_count) == -1)
			return (-1);
	}
	else if (is_redirection_token(parser->current->type))
	{
		if (handle_redir_in_parse(parser, node) == -1)
			return (-1);
	}
	else
	{
		if (parser->current->value)
			ft_fprintf(2, "Parse error: Unexpected token '%s'\n",
				parser->current->value);
		else
			ft_fprintf(2, "Parse error: Unexpected token '%s'\n", "NULL");
		return (-1);
	}
	return (0);
}

/* Check for syntax errors related to pipes */
int	check_pipe_syntax_errors(parser_t *parser)
{
	t_token	*current;

	current = parser->tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (current->next && current->next->type == TOKEN_PIPE)
			{
				ft_fprintf(2,
					"bash: syntax error near unexpected token `||'\n");
				return (-1);
			}
			if (!current->next || current->next->type == TOKEN_EOF)
			{
				ft_fprintf(2, "bash: syntax error near unexpected token `|'\n");
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
