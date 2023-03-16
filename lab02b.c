#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int value = 5;
    pid_t pid;

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == 0) { /* child process */
        close(fd[0]);
        value += 15;
        write(fd[1], &value, sizeof(value));
        close(fd[1]); 
        return 0;
    }
    else if (pid > 0) { 
        int status;
        close(fd[1]); 
        wait(&status);
        if (WIFEXITED(status)) {
            read(fd[0], &value, sizeof(value));
            printf("PARENT: value = %d\n", value);
        }
        close(fd[0]); 
        return 0;
    }
    else { 
        perror("fork");
        return 1;
    }
}
