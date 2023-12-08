#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    int cnt = 0;
    while (1) {
        sleep(10);
        cnt++;

        if (cnt % 6 == 0) {
            // Get time
            time_t t = time(NULL);
            struct tm tm;
            localtime_r(&t, &tm);
            printf("tm_sec: %d\n", tm.tm_sec);
        }

        // Run for 3+ hours
        if (cnt >= (3 * 60 * 60) / 10) {
            break;
        }
    }

    return 0;
}
