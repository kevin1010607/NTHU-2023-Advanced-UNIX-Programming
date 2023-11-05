#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    // Check if fork() returns an error
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    // Child process
    if(pid == 0) {
        printf("Child  process pid: %d\n", getpid());
        exit(0);
    }
    // Parent process
    else {
        printf("Parent process pid: %d\n", getpid());
        // Parent hangs for a while to make sure that child is finished 
        // and can be seen as zombie
        sleep(3);
        // Execute 'ps' command, 'Z' state indicates a zombie process
        char command[100] = {};
        sprintf(command, "ps -o pid,ppid,state -p %d", pid);
        system(command); 
    }

    return 0;
}
