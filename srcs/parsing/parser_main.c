/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:32 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 23:00:57 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* NEW: Initialize command parsing variables and node */
int	initialize_command_parsing(t_parser *parser, char ***args, int *arg_count,
		t_ast_node **node)
{
	if (!parser->current || parser->current->type == TOKEN_EOF
		|| parser->current->type == TOKEN_PIPE)
		return (1);
	*args = NULL;
	*arg_count = 0;
	*node = init_command_node();
	if (!*node)
	{
		return (-1);
	}
	return (0);
}

/* NEW: Finalize command parsing,
	checking for empty command and assigning args */
int	finalize_command_parsing(char ***args, int *arg_count, t_ast_node **node)
{
	if (*arg_count == 0 && !(*node)->u_data.command.redirs)
	{
		free_ast(*node);
		*node = NULL;
		return (-1);
	}
	(*node)->u_data.command.args = *args;
	return (0);
}

/* Main parse_command function - HEREDOCS ARE READ DURING PARSING */
t_ast_node	*parse_command(t_parser *parser)
{
	char		**args;
	int			arg_count;
	t_ast_node	*node;
	int			ret;

	ret = initialize_command_parsing(parser, &args, &arg_count, &node);
	if (ret == 1 || ret == -1)
		return (NULL);
	while (parser->current && parser->current->type != TOKEN_EOF
		&& parser->current->type != TOKEN_PIPE)
	{
		if (process_command_tokens(parser, node, &args, &arg_count) == -1)
		{
			free_str_array(args);
			free_ast(node);
			return (NULL);
		}
	}
	if (finalize_command_parsing(&args, &arg_count, &node) == -1)
		return (NULL);
	return (node);
}

/* Parse pipeline with proper error handling */
t_ast_node	*parse_pipeline(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	left = parse_command(parser);
	if (!left)
		return (NULL);
	while (parser->current && parser->current->type == TOKEN_PIPE)
	{
		parser->current = parser->current->next;
		right = parse_command(parser);
		if (!right)
		{
			error_syntax_context(NULL, left->u_data.command.args[0]);
			free_ast(left);
			return (NULL);
		}
		pipe_node = create_pipe_node(left, right);
		if (!pipe_node)
			return (NULL);
		left = pipe_node;
	}
	return (left);
}

/* Main parse function - heredocs already processed during parsing */
t_ast_node	*parse(t_token *tokens)
{
	t_parser	parser;

	parser.tokens = tokens;
	parser.current = tokens;
	if (check_pipe_syntax_errors(&parser) == -1)
		return (NULL);
	return (parse_pipeline(&parser));
}
