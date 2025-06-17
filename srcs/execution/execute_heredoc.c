#include "minishell.h"

/* In your execution function, use this for heredoc: */
void	handle_heredoc_in_execution(redir_t *redir)
{
	if (redir->type == REDIR_HEREDOC)
	{
		setup_heredoc_for_execution(redir);
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
	}
}
