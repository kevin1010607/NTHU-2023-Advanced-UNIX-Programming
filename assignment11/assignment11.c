#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "daemonize.h"

int main() {
    // (1) Call the daemonize function correctly.
    daemonize("assignment11_daemon");
    
    // (2) Create a text file and write the login name
    FILE* file = fopen("assignment11.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    char* login_name = getlogin();
    fprintf(file, "Login name: %s\n", (login_name != NULL) ? login_name : "Not logged in");
    fclose(file);

    // Process does its daemon-specific tasks here...

    return 0;
}
