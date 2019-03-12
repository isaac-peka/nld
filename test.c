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
        perror("open:");
        exit(1);
    }

    Elf_State * state = calloc(1, sizeof(Elf_State));
    state->s_fd = fd;

    Elf_ErrNo errno;

    errno = Elf_read_ehdr(state);
    if (errno != Elf_OK) {
        Elf_print_error(1, "Error reading ehdr:", errno);
        exit(1);
    }

    errno = Elf_read_shdrs(state);
    if (errno != Elf_OK) {
        Elf_print_error(1, "Error reading shdrs:", errno);
        exit(1);
    }

    errno = Elf_read_shstrtab(state);
    if (errno != Elf_OK) {
        Elf_print_error(1, "Error reading shstrtab:", errno);
        exit(1);
    }


    Elf32_Ehdr * ehdr = state->s_ehdr;

    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        Elf32_Shdr * shdr = &state->s_shdrs[i];
        char * name = &state->s_shstrtab[shdr->sh_name];

        printf("Section: %s\n", name);
    }

    close(fd);
}
