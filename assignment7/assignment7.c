#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    // Check if fork() returns an error
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    // Parent process
    else if (pid > 0) {
        printf("Parent process pid: %d\n", getpid());
        // Parent waits for the child
        wait(NULL);
    }
    // Child process
    else {
        printf("Child  process pid: %d\n", getpid());
        // Create a new session
        if (setsid() == -1) { 
            perror("setsid failed");
            exit(1);
        }
        // Verify that the child is a process group leader and has no controlling terminal
        char command[100] = {};
        sprintf(command, "ps -o pid,pgid,tpgid -p %d", getpid());
        system(command);
    }

    return 0;
}
