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

    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("Bad open:");
        exit(1);
    }

    Elf_ErrNo err;

    Elf32_Ehdr * ehdr = Elf_read_ehdr(fd, &err);
    if (ehdr == NULL) {
        Elf_print_error(1, "Error reading ehdr:", err);
        exit(1);
    }

    Elf32_Shdr * shdrs = Elf_read_shdrs(fd, ehdr, &err);
    if (ehdr == NULL) {
        Elf_print_error(1, "Error reading shdrs:", err);
        exit(1);
    }

    Elf32_Shdr * shstrtab = &shdrs[ehdr->e_shstrndx];

    char * strtab = Elf_read_section(fd, shstrtab, &err);
    if (strtab == NULL) {
        Elf_print_error(1, "Error reading strtab:", err);
        exit(1);
    }

    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        Elf32_Shdr * shdr = &shdrs[i];

        char * section_name = &strtab[shdr->sh_name];

        printf("Section %s\n", section_name);
        Elf_print_shdr(1, shdr);
        putchar('\n');
    }
}
