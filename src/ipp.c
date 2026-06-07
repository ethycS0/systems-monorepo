#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_pipeline(char *cmd1[], char *cmd2[]) {
        pid_t pid1, pid2;
        int pipefd[2];

        if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
                return -1;
        }

        if (pipe(pipefd) == -1) {
                return -1;
        }

        pid1 = fork();
        if (pid1 < 0) {
                exit(1);
        } else if (pid1 == 0) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
                execvp(cmd1[0], cmd1);
                exit(1);
        }

        pid2 = fork();
        if (pid2 < 0) {
                exit(1);
        } else if (pid2 == 0) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
                execvp(cmd2[0], cmd2);
                exit(1);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        int status;
        waitpid(pid1, &status, 0);
        waitpid(pid1, &status, 0);

        return 0;
}
