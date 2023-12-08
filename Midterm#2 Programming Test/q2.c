#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#define N 1000

void printSymbolicLinks(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullpath[N], linkpath[N];
    ssize_t bytes;

    if ((dir = opendir(path)) == NULL) return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        // Build the full path
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        // Check if the entry is a symbolic link
        if (lstat(fullpath, &statbuf) != 0) continue;

        if (S_ISLNK(statbuf.st_mode)) {
            bytes = readlink(fullpath, linkpath, sizeof(linkpath) - 1);

            if (bytes != -1) {
                linkpath[bytes] = '\0';
                printf("%s\n", linkpath);
            }
        }

        // Recursively call the function if it is a directory, 
        if (S_ISDIR(statbuf.st_mode)) {
            printSymbolicLinks(fullpath);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Invalid number of arguments.");
        exit(1);
    }

    const char *path = argv[1];
    struct stat statbuf;
    char linkpath[N];
    ssize_t bytes;

    if (lstat(path, &statbuf) != 0) exit(1);

    // Check if the given path is a symbolic link
    if (S_ISLNK(statbuf.st_mode)) {
        bytes = readlink(path, linkpath, sizeof(linkpath) - 1);
        if (bytes != -1) {
            linkpath[bytes] = '\0';
            printf("%s\n", linkpath);
        }
    }

    if (S_ISDIR(statbuf.st_mode)) {
        printSymbolicLinks(path);
    }

    return 0;
}
