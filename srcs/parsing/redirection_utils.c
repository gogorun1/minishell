/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding <wding@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:52:14 by wding             #+#    #+#             */
/*   Updated: 2025/06/17 23:13:56 by wding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helper function to add a redirection to the command's list */
void	add_redirection(t_command *command_data, t_redir_type type,
		char *filename)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		perror("malloc error for redirection");
		exit(EXIT_FAILURE);
	}
	new_redir->type = type;
	new_redir->file = ft_strdup(filename);
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
int	process_heredoc_during_parse(t_redir *redir)
{
	redir->heredoc_content = read_heredoc_content(redir->file);
	if (!redir->heredoc_content)
		return (-1);
	return (0);
}

/* Process all heredocs in a command during parsing */
int	process_command_heredocs_during_parse(t_command *cmd)
{
	t_redir	*current;

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
t_redir	*find_last_redirection(t_ast_node *node)
{
	t_redir	*last_redir;

	last_redir = node->u_data.command.redirs;
	while (last_redir->next)
		last_redir = last_redir->next;
	return (last_redir);
}

/* Process heredoc after adding redirection */
int	process_heredoc_redirection(t_ast_node *node, t_redir_type type)
{
	t_redir	*last_redir;

	if (type == REDIR_HEREDOC)
	{
		last_redir = find_last_redirection(node);
		if (process_heredoc_during_parse(last_redir) == -1)
			return (-1);
	}
	return (0);
}
