#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd1[2], fd2[2];
    pid_t pid;

    char parent_msg[] = "Hello child!";
    char child_msg[] = "Hello parent!";
    char buffer[100];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        /* Child process */

        printf("I am in child process\n");

        close(fd1[1]);
        close(fd2[0]);

        read(fd1[0], buffer, sizeof(buffer) - 1);
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Message from parent: %s\n", buffer);

        write(fd2[1], child_msg, strlen(child_msg) + 1);

        close(fd1[0]);
        close(fd2[1]);
    }
    else {
        /* Parent process */

        printf("I am in parent process\n");

        close(fd1[0]);
        close(fd2[1]);

        write(fd1[1], parent_msg, strlen(parent_msg) + 1);

        close(fd1[1]);

        read(fd2[0], buffer, sizeof(buffer) - 1);
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Message from child: %s\n", buffer);

        close(fd2[0]);

        wait(NULL);
    }

    return 0;
}
