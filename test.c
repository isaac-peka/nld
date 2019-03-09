#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "elf.h"


void main(int argc, char ** argv) {
    if (argc < 2) {
        puts("Usage: test [path]");
        exit(1);
    }

    char * path = argv[1];

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open: ");
        exit(1);
    }


    Elf32_Ehdr * hdr = Elf_read_header(fd);

    Elf_print_header(1, hdr);

    free(hdr);
    close(fd);
}
