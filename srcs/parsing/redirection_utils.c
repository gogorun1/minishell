/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:14 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 17:52:16 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helper function to add a redirection to the command's list */
void	add_redirection(command_t *command_data, redir_type_t type,
		char *filename)
{
	redir_t	*new_redir;
	redir_t	*current;

	new_redir = malloc(sizeof(redir_t));
	if (!new_redir)
	{
		perror("malloc error for redirection");
		exit(EXIT_FAILURE);
	}
	new_redir->type = type;
	new_redir->file = strdup(filename);
	new_redir->heredoc_content = NULL;
	new_redir->next = NULL;
	if (!command_data->redirs)
		command_data->redirs = new_redir;
	else
	{
		current = command_data->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

/* Process heredoc during parsing - BEFORE any forks */
int	process_heredoc_during_parse(redir_t *redir)
{
	redir->heredoc_content = read_heredoc_content(redir->file);
	if (!redir->heredoc_content)
		return (-1);
	return (0);
}

/* Process all heredocs in a command during parsing */
int	process_command_heredocs_during_parse(command_t *cmd)
{
	redir_t	*current;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			if (process_heredoc_during_parse(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

/* Find last redirection in the list */
redir_t	*find_last_redirection(ast_node_t *node)
{
	redir_t	*last_redir;

	last_redir = node->data.command.redirs;
	while (last_redir->next)
		last_redir = last_redir->next;
	return (last_redir);
}

/* Process heredoc after adding redirection */
int	process_heredoc_redirection(ast_node_t *node, redir_type_t type)
{
	redir_t	*last_redir;

	if (type == REDIR_HEREDOC)
	{
		last_redir = find_last_redirection(node);
		if (process_heredoc_during_parse(last_redir) == -1)
			return (-1);
	}
	return (0);
}
