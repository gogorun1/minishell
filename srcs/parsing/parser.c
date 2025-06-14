#include "minishell.h"

// // Helper function to add a redirection to the command's list
// void add_redirection(command_t *command_data, redir_type_t type, char *filename) {
//     redir_t *new_redir = malloc(sizeof(redir_t));
//     if (!new_redir) {
//         perror("malloc error for redirection");
//         exit(EXIT_FAILURE); // Or handle error more gracefully
//     }
//     new_redir->type = type;
//     new_redir->file = strdup(filename); // Duplicate filename to own the memory
//     new_redir->next = NULL;

//     if (!command_data->redirs) {
//         command_data->redirs = new_redir;
//     } else {
//         redir_t *current = command_data->redirs;
//         while (current->next) {
//             current = current->next;
//         }
//         current->next = new_redir;
//     }
//     // printf("        Added redirection: type %d, file '%s'\n", type, filename);
// }

// ast_node_t *parse_command(parser_t *parser) {
//     // If current token is EOF or PIPE, it's not the start of a command
//     if (!parser->current || parser->current->type == TOKEN_EOF ||
//         parser->current->type == TOKEN_PIPE) {
//         // printf("    parse_command: No command or pipe/EOF encountered, returning NULL.\n");
//         return NULL;
//     }

//     char **args = NULL;
//     int arg_count = 0;
//     ast_node_t *node = NULL; // Initialize node to NULL

//     // printf("    Parsing command/redirections starting with: '%s' (type: %d)\n",
//         //    parser->current->value ? parser->current->value : "NULL", parser->current->type);

//     // Allocate the command node early
//     node = malloc(sizeof(ast_node_t));
//     if (!node) {
//         perror("malloc error for AST node");
//         return NULL;
//     }
//     node->type = AST_COMMAND;
//     node->data.command.args = NULL; // Initialize to NULL
//     node->data.command.redirs = NULL; // Initialize to NULL

//     // Loop to parse arguments and redirections until a pipe, EOF, or unexpected token
//     while (parser->current &&
//            parser->current->type != TOKEN_EOF &&
//            parser->current->type != TOKEN_PIPE) {

//         if (parser->current->type == TOKEN_WORD) {
//             // It's an argument: add to args list
//             args = realloc(args, sizeof(char*) * (arg_count + 2));
//             if (!args) {
//                 perror("realloc error for args");
//                 free_ast(node); // Clean up partially created node
//                 return NULL;
//             }
//             args[arg_count] = strdup(parser->current->value);
//             args[arg_count + 1] = NULL; // Null-terminate the argument list
//             // printf("      Added arg[%d]: '%s'\n", arg_count, args[arg_count]);
//             arg_count++;
//             parser->current = parser->current->next; // Consume the word token
//         } else if (parser->current->type == TOKEN_REDIRECT_IN) { // Handle '<' (input redirection)
//             parser->current = parser->current->next; // Consume '<'
//             if (parser->current && parser->current->type == TOKEN_WORD) {
//                 add_redirection(&(node->data.command), REDIR_IN, parser->current->value);
//                 parser->current = parser->current->next; // Consume filename
//             } else {
//                 fprintf(stderr, "Parse error: Expected filename after '<'\n");
//                 free_ast(node); // Clean up partially created node
//                 return NULL; // Syntax error
//             }
//         } else if (parser->current->type == TOKEN_REDIRECT_OUT) { // Handle '>' (output redirection)
//             parser->current = parser->current->next; // Consume '>'
//             if (parser->current && parser->current->type == TOKEN_WORD) {
//                 add_redirection(&(node->data.command), REDIR_OUT, parser->current->value);
//                 parser->current = parser->current->next; // Consume filename
//             } else {
//                 fprintf(stderr, "Parse error: Expected filename after '>'\n");
//                 free_ast(node);
//                 return NULL;
//             }
//         } else if (parser->current->type == TOKEN_APPEND) { // Handle '>>' (append output redirection)
//             parser->current = parser->current->next; // Consume '>>'
//             if (parser->current && parser->current->type == TOKEN_WORD) {
//                 add_redirection(&(node->data.command), REDIR_APPEND, parser->current->value);
//                 parser->current = parser->current->next; // Consume filename
//             } else {
//                 fprintf(stderr, "Parse error: Expected filename after '>>'\n");
//                 free_ast(node);
//                 return NULL;
//             }
// 		} else if (parser->current->type == TOKEN_HEREDOC) { // Handle '<<' (heredoc)
//             parser->current = parser->current->next; // Consume '<<'
//             if (parser->current && parser->current->type == TOKEN_WORD) {
//                 // 使用 filename 字段存储 delimiter
//                 add_redirection(&(node->data.command), REDIR_HEREDOC, parser->current->value);
//                 parser->current = parser->current->next; // Consume delimiter
//             } else {
//                 fprintf(stderr, "Parse error: Expected filename after '<<'\n");
//                 free_ast(node);
//                 return NULL;
//             }
// // ...
//         // Add more cases for other redirection types like TOKEN_LTLT (heredoc) if needed
//         // } else if (parser->current->type == TOKEN_LTLT) { ... }
//         } else {
//             // Unexpected token for a command part (neither word nor redirection)
//             fprintf(stderr, "Parse error: Unexpected token '%s' (type: %d) in command\n",
//                     parser->current->value ? parser->current->value : "NULL", parser->current->type);
//             free_ast(node); // Clean up partially created node
//             return NULL;
//         }
//     }

//     // A command must have at least one argument OR at least one redirection.
//     // If neither is present, it's not a valid command.
//     if (arg_count == 0 && !node->data.command.redirs) {
//         // printf("    parse_command: No arguments or redirections found, returning NULL.\n");
//         free_ast(node); // Free the allocated node as it's empty
//         return NULL;
//     }

//     node->data.command.args = args; // Assign the collected arguments
//     // node->data.command.redirs is already populated by add_redirection

//     // printf("    Created command node with %d args and %s redirections\n",
//         //    arg_count, node->data.command.redirs ? "some" : "no");
//     return node;
// }

// ast_node_t *parse_pipeline(parser_t *parser) {
//     printf("  Starting pipeline parsing\n");
    
//     // Parse first command
//     ast_node_t *left = parse_command(parser);
//     if (!left) {
//         // printf("  No left command found\n");
//         return NULL;
//     }
    
//     // printf("  Got left command, current token: %s\n", 
//     //         parser->current && parser->current->value ? parser->current->value : "NULL/EOF");
    
//     // Keep parsing pipes
//     while (parser->current && parser->current->type == TOKEN_PIPE) {
//         printf("  Found pipe! Continuing pipeline...\n");
//         parser->current = parser->current->next; // Skip the pipe token
        
//         ast_node_t *right = parse_command(parser);
//         if (!right) {
//             // printf("  ERROR: No command after pipe\n");
//             // free_ast(left); // Commented out for demo
//             return NULL;
//         }
        
//         printf("  Got right command, creating pipe node\n");
        
//         // Create pipe node with left and right
//         ast_node_t *pipe_node = malloc(sizeof(ast_node_t));
//         pipe_node->type = AST_PIPE;
//         pipe_node->data.binary.left = left;
//         pipe_node->data.binary.right = right;
        
//         // The pipe node becomes the new left for the next iteration
//         left = pipe_node;
        
//         // printf("  Pipe node created, checking for more pipes...\n");
//     }
    
//     printf("  Pipeline parsing complete\n");
//     return left;
// }

// ast_node_t *parse(t_token *tokens) {
//     parser_t parser;
//     parser.tokens = tokens;
//     parser.current = tokens;
    
//     return parse_pipeline(&parser);
// }

#include "minishell.h"

/* Helper function to add a redirection to the command's list */
void	add_redirection(command_t *command_data, redir_type_t type, char *filename)
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
	new_redir->heredoc_content = NULL; // Add this field to redir_t
	new_redir->next = NULL;
	if (!command_data->redirs)
	{
		command_data->redirs = new_redir;
	}
	else
	{
		current = command_data->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

/* Read heredoc content interactively - THIS HAPPENS DURING PARSING */
char	*read_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;
	char	*temp;

	content = strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			// Bash warning for unexpected EOF
			write(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file (wanted `", 64);
			write(STDERR_FILENO, delimiter, strlen(delimiter));
			write(STDERR_FILENO, "')\n", 3);
			break; // Use content collected so far
		}
		if (errno == EINTR)
		{
			free(line);
			free(content);
			g_signal_status = 130;
			return (NULL);
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Append line to content
		temp = malloc(strlen(content) + strlen(line) + 2);
		if (!temp)
		{
			free(line);
			free(content);
			return (NULL);
		}
		strcpy(temp, content);
		strcat(temp, line);
		strcat(temp, "\n");
		free(content);
		free(line);
		content = temp;
	}
	return (content);
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

/* Handle redirection parsing - READS HEREDOC CONTENT IMMEDIATELY */
static int	handle_redirection(parser_t *parser, ast_node_t *node)
{
	redir_type_t	type;
	
	if (parser->current->type == TOKEN_REDIRECT_IN)
		type = REDIR_IN;
	else if (parser->current->type == TOKEN_REDIRECT_OUT)
		type = REDIR_OUT;
	else if (parser->current->type == TOKEN_APPEND)
		type = REDIR_APPEND;
	else if (parser->current->type == TOKEN_HEREDOC)
		type = REDIR_HEREDOC;
	else
		return (-1);
	
	parser->current = parser->current->next; // Consume redirection token
	if (!parser->current || parser->current->type != TOKEN_WORD)
	{
		fprintf(stderr, "Parse error: Expected filename after redirection\n");
		return (-1);
	}
	
	add_redirection(&(node->data.command), type, parser->current->value);
	
	// FOR HEREDOC: Read content immediately during parsing
	if (type == REDIR_HEREDOC)
	{
		redir_t *last_redir = node->data.command.redirs;
		while (last_redir->next)
			last_redir = last_redir->next;
		
		if (process_heredoc_during_parse(last_redir) == -1)
			return (-1);
	}
	
	parser->current = parser->current->next; // Consume filename/delimiter
	return (0);
}

/* Handle word token (arguments) */
static char	**handle_word_token(parser_t *parser, char **args, int *arg_count)
{
	char	**new_args;

	new_args = realloc(args, sizeof(char*) * (*arg_count + 2));
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

/* Check if current token is a redirection */
static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
			type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

/* Initialize command node */
static ast_node_t	*init_command_node(void)
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

/* Main parse_command function - HEREDOCS ARE READ DURING PARSING */
ast_node_t	*parse_command(parser_t *parser)
{
	char		**args;
	int			arg_count;
	ast_node_t	*node;

	if (!parser->current || parser->current->type == TOKEN_EOF ||
		parser->current->type == TOKEN_PIPE)
		return (NULL);
	
	args = NULL;
	arg_count = 0;
	node = init_command_node();
	if (!node)
		return (NULL);
	while (parser->current && parser->current->type != TOKEN_EOF &&
		   parser->current->type != TOKEN_PIPE)
	{
		if (parser->current->type == TOKEN_WORD)
		{
			args = handle_word_token(parser, args, &arg_count);
			if (!args)
			{
				free_ast(node);
				return (NULL);
			}
		}
		else if (is_redirection_token(parser->current->type))
		{
			// This will read heredoc content if it's a heredoc
			if (handle_redirection(parser, node) == -1)
			{
				free_ast(node);
				return (NULL);
			}
		}
		else
		{
			fprintf(stderr, "Parse error: Unexpected token '%s'\n",
					parser->current->value ? parser->current->value : "NULL");
			free_ast(node);
			return (NULL);
		}
	}
	if (arg_count == 0 && !node->data.command.redirs)
	{
		free_ast(node);
		return (NULL);
	}
	node->data.command.args = args;
	return (node);
}

ast_node_t	*parse_pipeline(parser_t *parser)
{
	ast_node_t	*left;
	ast_node_t	*right;
	ast_node_t	*pipe_node;

	left = parse_command(parser);
	if (!left)
		return (NULL);
	
	while (parser->current && parser->current->type == TOKEN_PIPE)
	{
		parser->current = parser->current->next;
		right = parse_command(parser);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		
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
		left = pipe_node;
	}
	return (left);
}

/* Main parse function - heredocs already processed during parsing */
ast_node_t	*parse(t_token *tokens)
{
	parser_t	parser;

	parser.tokens = tokens;
	parser.current = tokens;
	
	return (parse_pipeline(&parser));
}

/* EXECUTION PHASE: Create pipe from heredoc content */
int	setup_heredoc_for_execution(redir_t *redir)
{
	int	pipefd[2];
	
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	
	// Write the stored content to pipe
	write(pipefd[1], redir->heredoc_content, strlen(redir->heredoc_content));
	close(pipefd[1]); // Close write end
	
	redir->heredoc_fd = pipefd[0]; // Store read end
	return (0);
}

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
