#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FILE_MODE 0644
#define COPYINCR (1024 * 1024l * 1024)

#define err_sys(fmt, ...) fprintf(stderr, (fmt "\n"), ##__VA_ARGS__)
#define err_quit(fmt, ...) do { err_sys(fmt, ##__VA_ARGS__); exit(1); } while(0)

int main(int argc, char *argv[])
{
    int fdin, fdout;
    void *src, *dst;
    size_t copysz;
    struct stat sbuf;
    off_t fsz = 0;

    if (argc != 3)
        err_quit("usage: %s <fromfile> <tofile>", argv[0]);

    if ((fdin = open(argv[1], O_RDONLY)) < 0)
        err_sys("can’t open %s for reading", argv[1]);

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("can’t creat %s for writing", argv[2]);

    if (fstat(fdin, &sbuf) < 0)         /* need size of input file */
        err_sys("fstat error");

    if (ftruncate(fdout, sbuf.st_size) < 0) /* set output file size */
        err_sys("ftruncate error");

    while (fsz < sbuf.st_size) {
        if ((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        else
            copysz = sbuf.st_size - fsz;

        /* TODO: Copy the file using mmap here */
        // Find the page_offset which is page aligned and comes before the fsz
        off_t page_offset = fsz & ~(sysconf(_SC_PAGE_SIZE) - 1);

        // Memory-map the input file and output file
        if ((src = mmap(NULL, copysz + fsz - page_offset, PROT_READ, MAP_PRIVATE, fdin, page_offset)) == MAP_FAILED)
            err_sys("mmap error for input");
        if ((dst = mmap(NULL, copysz + fsz - page_offset, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, page_offset)) == MAP_FAILED)
            err_sys("mmap error for output");

        // Copy the data
        memcpy(dst + fsz - page_offset, src + fsz - page_offset, copysz);

        // Unmap the memory
        if (munmap(src, copysz + fsz - page_offset) < 0)
            err_sys("munmap error for input");
        if (munmap(dst, copysz + fsz - page_offset) < 0)
            err_sys("munmap error for output");

        fsz += copysz;
    }

    // Close the file descriptors
    close(fdin);
    close(fdout);

    return 0;
}
