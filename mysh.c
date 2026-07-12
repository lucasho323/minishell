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
    char *argv[MAXARGS];                     // ← 新增：切好的參數放這

    while (1) {
        printf("mysh> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;                           // Ctrl+D 會走到這裡
        line[strcspn(line, "\n")] = '\0';    // 把結尾的換行去掉

        if (split(line, argv) == 0)          // ← 改：切字串，零段（空行）就跳過
            continue;

        pid_t pid = fork();                  // 生分身
        if (pid == 0) {
            execvp(argv[0], argv);           // ← 改：程式名是 argv[0]，陣列是切好的
            printf("找不到指令: %s\n", argv[0]);
            return 1;
        } else {
            wait(NULL);                      // 本尊等分身
        }
    }
    return 0;
}