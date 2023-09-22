#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define N 100

int main() {
    // Define string
    char buffer[N];
    int start_offset = 14;
    char old_str[N] = "student.";
    char new_str[N] = "NTHU student.";
    int len_old_str = strlen(old_str);
    int len_new_str = strlen(new_str);

    // Open the file using O_RDWR flag
    int file_id = open("sample.txt", O_RDWR);
    if(file_id == -1) {
        printf("Failed to open the file.");
        return 1;
    }

    // Use lseek and read to print “student.” in the command line
    lseek(file_id, start_offset, SEEK_SET);
    read(file_id, buffer, len_old_str * sizeof(char));
    buffer[len_old_str] = '\0';
    printf("%s\n", buffer);

    // Use lseek and write to replace “student.” with “NTHU student.”
    lseek(file_id, start_offset, SEEK_SET);
    write(file_id, new_str, len_new_str);
    write(file_id, "\n", 1); // add a "\n" in the end.

    // Print the whole sentence in the file in the command line
    lseek(file_id, 0, SEEK_SET);
    ssize_t bytes_read = read(file_id, buffer, sizeof(buffer));
    if(bytes_read == -1) {
        printf("Failed to read the file.");
        return 1;
    }
    buffer[bytes_read] = '\0';
    printf("%s", buffer);

    // Close the file
    close(file_id);

    return 0;
}

