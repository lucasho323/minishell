#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 64

int split(char *line, char *argv[]) {
    int n = 0;
    char *tok = strtok(line, " ");
    while (tok != NULL && n < MAXARGS - 1) {
        argv[n] = tok;
        n++;
        tok = strtok(NULL, " ");
    }
    argv[n] = NULL;
    return n;
}

int main(void) {
    char line[256];
    char *argv[MAXARGS];

    while (1) {
        printf("mysh> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        line[strcspn(line, "\n")] = '\0';

        if (split(line, argv) == 0)
            continue;

        if (strcmp(argv[0], "cd") == 0) {        // 攔截:本尊自己做
            if (argv[1] == NULL)
                printf("cd: 缺少路徑\n");
            else if (chdir(argv[1]) != 0)
                perror("cd");
            continue;                            // 不 fork,回去讀下一行
        }

        if (strcmp(argv[0], "exit") == 0)        // 攔截:本尊自己結束
            break;

        pid_t pid = fork();
        if (pid == 0) {
            execvp(argv[0], argv);
            printf("找不到指令: %s\n", argv[0]);
            return 1;
        } else {
            wait(NULL);
        }
    }
    return 0;
}