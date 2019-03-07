#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "elf.h"
#include "elftype.h"


Elf_ErrNo Elf_validate_elf_header(Elf32_Ehdr * elf_hdr) {
    char * e_ident = elf_hdr->e_ident;

    if (strncmp(e_ident, "\x7f""ELF", 4) != 0) {
        return Elf_INVALID_HDR;
    }

    // for now we only support x86 32

    if (e_ident[EI_CLASS] != ELFCLASS32 && 
            e_ident[EI_DATA] != ELFDATA2LSB) {

        return Elf_INVALID_ARCH;
    }

    return Elf_OK;
}



Elf32_Ehdr * Elf_read_elf_header(int fd) {
    if (lseek(fd, 0, SEEK_SET) == -1) {
        return NULL;
    }

    Elf32_Ehdr * elf_hdr = calloc(1, sizeof(Elf32_Ehdr));

    if (read(fd, elf_hdr, sizeof(elf_hdr)) == -1) {
        free(elf_hdr);
        return NULL;
    }

    return elf_hdr;
}
