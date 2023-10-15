#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory structure for file stream
typedef struct {
    char *buf;
    size_t size;
    size_t pos;
} MemStream;

// Function to read from the memory stream
int read_mem(void *cookie, char *buf, int size) {
    MemStream *stream = (MemStream*)cookie;

    if(stream->pos + size > stream->size)
        size = stream->size - stream->pos;

    memcpy(buf, stream->buf + stream->pos, size);
    stream->pos += size;

    return size;
}

// Function to write to the memory stream
int write_mem(void *cookie, const char *buf, int size) {
    MemStream *stream = (MemStream*)cookie;

    if(stream->pos + size > stream->size)
        size = stream->size - stream->pos;

    memcpy(stream->buf + stream->pos, buf, size);
    stream->pos += size;

    return size;
}

// Function to seek within the memory stream
fpos_t seek_mem(void *cookie, fpos_t offset, int whence) {
    fpos_t npos;
    MemStream *stream = (MemStream*)cookie;

    switch (whence) {
        case SEEK_SET:
            npos = offset;
            break;
        case SEEK_CUR:
            npos = stream->pos + offset;
            break;
        case SEEK_END:
            npos = stream->size + offset;
            break;
        default:
            return -1;
    }

    if(npos < 0 || npos > stream->size)
        return -1;

    stream->pos = npos;

    return npos;
}

// Function to close the memory stream
int close_mem(void *cookie) {
    MemStream *stream = (MemStream*)cookie;

    // if(stream->buf)
    //     free(stream->buf);
    free(stream);

    return 0;
}

// Function to open a memory stream
FILE* fmemopen(void *buf, size_t size, const char *mode) {
    MemStream* stream = (MemStream*)calloc(sizeof(MemStream), 1);

    stream->buf = (char*)buf;
    stream->size = size;
    stream->pos = 0;

    return funopen(stream, read_mem, write_mem, seek_mem, close_mem);
}

int main() {
    // (1) Write "hello, world" into the file stream.
    char buffer[100] = {};
    FILE* stream = fmemopen(buffer, sizeof(buffer), "w");
    fprintf(stream, "hello, world");

    // (2) Seek the position of "world" in the file stream.
    fseek(stream, 7, SEEK_SET);

    // (3) Read the word "world" from the file stream and print it.
    char word[20] = {};
    fread(word, sizeof(char), 5, stream);
    printf("%s\n", word);

    // Print the whole sentence "hello, world".
    char sentence[20] = {};
    fseek(stream, 0, SEEK_SET);
    fread(sentence, sizeof(char), 12, stream);
    printf("%s\n", sentence);

    // (4) Close the file stream correctly.
    fclose(stream);

    return 0;
}
