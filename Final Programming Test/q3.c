#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void alarm_handler(int signum) {
    printf("Alarm!\n");
}

void setAlarm(int sec) {
    struct itimerval timer;
    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}

void clearAlarm() {
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}

int main(void) {
    // Set up the signal handler
    struct sigaction sa;
    sa.sa_handler = &alarm_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    // Test cases
    setAlarm(2); // Set 2 sec alarm at 0s, will finish at 2s after execution
    sleep(1);
    setAlarm(6); // Set 6 sec alarm at 1s, will finish at 7s after execution
    sleep(1);
    setAlarm(3); // Set 3 sec alarm at 2s, will finish at 5s after execution
    sleep(4);
    clearAlarm(); // Clear all alarms at 6s after execution

    return 0;
}
