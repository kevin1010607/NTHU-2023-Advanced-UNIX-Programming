#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static volatile sig_atomic_t sigflag; /* set nonzero by sig handler */
static sigset_t newmask, oldmask, zeromask;

static void
sig_usr(int signo) /* one signal handler for SIGUSR1 and SIGUSR2 */
{
    sigflag = 1;
}

static void TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR1) error");

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR2) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    /* Block SIGUSR1 and SIGUSR2, and save current signal mask */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("SIG_BLOCK error");
}

static void TELL_PARENT(void)
{
    kill(getppid(), SIGUSR2); /* tell parent we’re done */
}

static void WAIT_PARENT(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask); /* and wait for parent */
    sigflag = 0;
    /* Reset signal mask to original value */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        perror("SIG_SETMASK error");
}

static void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1); /* tell child we’re done */
}

static void WAIT_CHILD(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask); /* and wait for child */
    sigflag = 0;
    /* Reset signal mask to original value */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        perror("SIG_SETMASK error");
}

static int increment_counter(FILE *const file)
{
    /* TODO */
    int counter;
    rewind(file);
    fscanf(file, "%d", &counter);
    counter++;
    rewind(file);
    fprintf(file, "%d\n", counter);
    fflush(file);
    return counter;
}

int main(void)
{
    /* TODO */
    pid_t pid;
    const char *filename = "output.txt";

    // Initialize the file with value 0
    FILE *init_file = fopen(filename, "w");
    if (init_file == NULL) {
        perror("Failed to open file for initialization");
        exit(1);
    }
    fprintf(init_file, "0\n");
    fclose(init_file);

    // Setup signal handling and mask
    TELL_WAIT();

    // Fork a new child
    pid = fork();

    // Fork failed
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    // Parent process
    else if (pid > 0) {
        FILE *output_file = fopen(filename, "r+");
        if (output_file == NULL) {
            perror("Failed to open file in parent process");
            exit(1);
        }
        for (int i = 0; i < 50; i++) {
            WAIT_CHILD();
            int counter = increment_counter(output_file);
            printf("Parent incrementing, value: %d\n", counter);
            TELL_CHILD(pid);
        }
        fclose(output_file);
        wait(NULL);
    }
    // Child process
    else {
        FILE *output_file = fopen(filename, "r+");
        if (output_file == NULL) {
            perror("Failed to open file in child process");
            exit(1);
        }
        for (int i = 0; i < 50; i++) {
            int counter = increment_counter(output_file);
            printf("Child incrementing, value: %d\n", counter);
            TELL_PARENT();
            WAIT_PARENT();
        }
        fclose(output_file);
    }
    return 0;
}
