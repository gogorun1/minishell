#include "minishell.h"

int execute_command(t_token *tokens, char **env)
{
    if (!tokens)
        return 1;
        
    // 构建参数数组
    int arg_count = count_tokens(tokens);
    char **args = malloc(sizeof(char *) * (arg_count + 1));
    if (!args)
        return 1;
        
    // 填充参数数组
    t_token *current = tokens;
    int i = 0;
    while (current)
    {
        args[i++] = ft_strdup(current->value);
        current = current->next;
    }
    args[i] = NULL;
    
    // 检查是否为内置命令
    if (is_builtin(args[0]))
        return execute_builtin(args, env);
        
    // 外部命令执行
    pid_t pid = fork();
    if (pid == 0) // 子进程
    {
        // 查找可执行文件
        char *path = find_executable(args[0], env);
        if (path)
        {
            execve(path, args, env);
            free(path);
        }
        
        // 根据命令名格式选择错误消息
        if (strchr(args[0], '/'))
            fprintf(stderr, "bash: %s: No such file or directory\n", args[0]);
        else
            fprintf(stderr, "bash: %s: command not found\n", args[0]);
            
        exit(127);
    }
    else if (pid > 0) // 父进程
    {
        int status;
        waitpid(pid, &status, 0);
        
        // 释放参数数组
        for (i = 0; args[i]; i++)
            free(args[i]);
        free(args);
        
        return WEXITSTATUS(status);
    }
    else // fork失败
    {
        perror("fork");
        return 1;
    }
}
