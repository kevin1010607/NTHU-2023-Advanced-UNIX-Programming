#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int agrc, char *argv[]) {
    time_t t = time(NULL);
    struct tm tm;
    char buffer[100] = {};

    localtime_r(&t, &tm);
    strftime(buffer, sizeof(buffer), "%H:%M:%S, %A %B %d, %Y", &tm);

    printf("%s\n", buffer);
    return 0;
}
