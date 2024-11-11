/*Write a C program to implement the following Unix/Linux command using fork, pipe, and exec system calls. Block the Ctrl-C and Ctrl-\ signals during execution.

Command to implement: ls -l | wc -l*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void block_signals() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

int main() {
    int pipe_fd[2];
    pid_t pid;

    block_signals();
    pipe(pipe_fd);

    pid = fork();
    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        execlp("ls", "ls", "-l", NULL);
        exit(0);
    } else {
        pid = fork();
        if (pid == 0) {
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            execlp("wc", "wc", "-l", NULL);
            exit(0);
        }
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}

/*10
*/