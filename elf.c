#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "elf.h"
#include "elftype.h"


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


Elf_ErrNo Elf_validate_elf_header(Elf32_Ehdr * elf_hdr) 
{
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


char * Elf_describe_e_type(Elf32_Half e_type) {
    switch (e_type) {
        case ET_NONE:
            return "No file type";
        case ET_REL:
            return "Relocatable file";
        case ET_EXEC:
            return "Executable file";
        case ET_DYN:
            return "Shared object file";
        case ET_CORE:
            return "Core file";
    }

    if (e_type >= ET_LOPROC) {
        return "Processor-specific";
    }

    return "Unknown";
}

char * Elf_describe_e_machine(Elf32_Half e_machine) {
    switch (e_machine) {
        case EM_NONE:
            return "No machine";
        case EM_M32:
            return "AT&T WE 32100";
        case EM_SPARC:
            return "SPARC";
        case EM_386:
            return "Intel 80386";
        case EM_68K:
            return "Motorola 68000";
        case EM_88K:
            return "Motorola 88000";
        case EM_860:
            return "Intel 80860";
        case EM_MIPS:
            return "MIPS RS3000";
        default:
            return "Unknown";
    }
}

char * Elf_describe_ei_class(uint8_t ei_class) {
    switch (ei_class) {
        case ELFCLASSNONE:
            return "Invalid class";
        case ELFCLASS32:
            return "32-bit objects";
        case ELFCLASS64:
            return "64-bit objects";
        default:
            return "Unknown";
    }
}