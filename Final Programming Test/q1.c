#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

void *handle_sigint(void *arg) {
    sigset_t set;
    int sig;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    sigwait(&set, &sig);
    printf("T1 handling SIGINT\n");
    return NULL;
}

void *handle_sigterm(void *arg) {
    sigset_t set;
    int sig;
    sigemptyset(&set);
    sigaddset(&set, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    sigwait(&set, &sig);
    printf("T2 handling SIGTERM\n");
    return NULL;
}

void *handle_sigusr1(void *arg) {
    sigset_t set;
    int sig;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    sigwait(&set, &sig);
    printf("T3 handling SIGUSR1\n");
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    pthread_create(&t1, NULL, handle_sigint, NULL);
    pthread_create(&t2, NULL, handle_sigterm, NULL);
    pthread_create(&t3, NULL, handle_sigusr1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
