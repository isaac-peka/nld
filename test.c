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

    Elf_State * state = Elf_open(argv[1]);
    if (state == NULL) {
        perror("open:");
        exit(1);
    }

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
    Elf32_Shdr * shstrtab = state->s_shstrtab;

    Elf_print_ehdr(1, ehdr);
    putchar('\n');

    Elf_print_shdr(1, shstrtab);
    putchar('\n');

    char * section_labels = malloc(shstrtab->sh_size);

    lseek(state->s_fd, shstrtab->sh_offset, SEEK_SET);
    read(state->s_fd, section_labels, shstrtab->sh_size);

    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        Elf32_Shdr * shdr = &state->s_shdrs[i];

        char * section_name = section_labels + shdr->sh_name;

        printf("Section %s\n", section_name);
        Elf_print_shdr(1, shdr);
        putchar('\n');
    }
}
