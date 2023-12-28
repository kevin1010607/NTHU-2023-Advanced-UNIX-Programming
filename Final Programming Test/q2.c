#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void sleep_us(unsigned int microseconds) {
    struct timeval tv;
    tv.tv_sec = microseconds / 1000000;
    tv.tv_usec = microseconds % 1000000;
    select(0, NULL, NULL, NULL, &tv);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Invalid Argument.");
        return 1;
    }
    unsigned int microseconds = atoi(argv[1]);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    sleep_us(microseconds);

    gettimeofday(&end, NULL);

    long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    printf("Sleep time: %ld us\n", elapsed);

    return 0;
}
