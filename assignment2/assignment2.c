#include <stdio.h>

void copyFile(const char *source_filename, const char *dest_filename) {
    // Open the source file in binary read mode
    FILE *source_file = fopen(source_filename, "rb");
    if (source_file == NULL) {
        printf("Error opening source file.\n");
        return;
    }

    // Open the dest file in binary write mode
    FILE *dest_file = fopen(dest_filename, "wb");
    if (dest_file == NULL) {
        printf("Error opening dest file.\n");
        fclose(source_file);
        return;
    }

    // Copy content character by character
    char ch;
    while ((ch = fgetc(source_file)) != EOF) {
        fputc(ch, dest_file);
    }

    // Close the files
    fclose(source_file);
    fclose(dest_file);

    printf("File copy successful.\n");
}

int main(int argc, char *argv[]) {
    // Check the number of paramter
    if (argc != 3) {
        printf("Error: This program requires exactly 2 parameters.\n");
        return 1;
    }

    // Copy the file from argv[1] to argv[2]
    copyFile(argv[1], argv[2]);

    return 0;
}