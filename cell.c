#include "cell.h"

int main()
{
	int status;
	char *line = NULL;

	while (1)
	{
		line = readline("prompt> ");
		if (line == NULL)
			break;
		if (fork() == 0)
			execvp(av[1], av + 1);
	}
	wait(&status);
	return EXIT_SUCCESS;
}