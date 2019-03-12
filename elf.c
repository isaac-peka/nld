#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "elf.h"



Elf_State * Elf_open(char * pathname) {
    int fd = open(pathname, O_RDONLY);

    if (fd == -1) {
        return NULL;
    }

    Elf_State * state = calloc(1, sizeof(Elf_State));
    return state;
}


void Elf_free_state(Elf_State * state) {
    free(state->s_ehdr);
    free(state->s_shdrs);
    free(state);
}


Elf_ErrNo Elf_read_ehdr(Elf_State * state) {
    int fd = state->s_fd;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        return Elf_BAD_READ;
    }

    Elf32_Ehdr * ehdr = malloc(sizeof(Elf32_Ehdr));

    if (ehdr == NULL) {
        return Elf_BAD_ALLOC;
    }

    if (read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        free(ehdr);
        return Elf_BAD_READ;
    }

    // Validate the elf header looks alright

    if (strncmp(ehdr->e_ident, "\x7f""ELF", 4) != 0) {
        free(ehdr);
        return Elf_EHDR_BAD_MAG;
    }

    // for now we only support x86 32b

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 &&
        ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {

        free(ehdr);
        return Elf_EHDR_BAD_ARCH;
    }

    state->s_ehdr = ehdr;
    return Elf_OK;
}


Elf_ErrNo Elf_read_shdrs(Elf_State * state) {
    Elf32_Ehdr * ehdr = state->s_ehdr;

    if (ehdr == NULL) {
        return Elf_NEED_EHDR;
    }

    Elf32_Off shoff = ehdr->e_shoff;

    if (shoff == 0) {
        return Elf_EHDR_BAD_SHOFF;
    }

    int fd = state->s_fd;

    if (lseek(fd, shoff, SEEK_SET) != shoff) {
        return Elf_BAD_READ;
    }

    if (ehdr->e_shentsize != sizeof(Elf32_Shdr)) {
        return Elf_EHDR_BAD_SHENTSIZE;
    }

    Elf32_Word shsize = ehdr->e_shnum * ehdr->e_shentsize;
    Elf32_Shdr * shdrs = malloc(shsize);

    if (shdrs == NULL) {
        return Elf_BAD_ALLOC;
    }

    if (read(fd, shdrs, shsize) != shsize) {
        free(shdrs);
        return Elf_BAD_READ;
    }

    state->s_shdrs = shdrs;
    return Elf_OK;
}


Elf_ErrNo Elf_read_shstrtab(Elf_State * state) {
    Elf32_Ehdr * ehdr = state->s_ehdr;
    if (ehdr == NULL) {
        return Elf_NEED_EHDR;
    }

    Elf32_Shdr * shdrs = state->s_shdrs;
    if (shdrs == NULL) {
        return Elf_NEED_SHDRS;
    }

    // Find the section header string table

    Elf32_Shdr * strtab_hdr = NULL;
    for (Elf32_Word i = 0; i < ehdr->e_shnum; i++) {
        Elf32_Shdr * shdr = &state->s_shdrs[i];

        if (shdr->sh_type == SHT_STRTAB &&
            shdr->sh_flags == 0) {

            strtab_hdr = shdr;
            break;
        }
    }

    // Not found

    if (strtab_hdr == NULL) {
        return Elf_NEED_SHSTRTAB;
    }

    // Read it in, make sure its the correct amount or fail

    int fd = state->s_fd;
    Elf32_Off sh_offset = strtab_hdr->sh_offset;

    if (lseek(fd, sh_offset, SEEK_SET) != sh_offset) {
        return Elf_BAD_READ;
    }

    Elf32_Word sh_size = strtab_hdr->sh_size;
    uint8_t * shstrtab = malloc(sh_size);

    if (read(fd, shstrtab, sh_size) != sh_size) {
        free(shstrtab);
        return Elf_BAD_READ;
    }

    state->s_shstrtab = shstrtab;
    return Elf_OK;
}


void Elf_print_ehdr(int fd, Elf32_Ehdr * hdr) {
    char * e_ident = hdr->e_ident;

    char header[] = "ELF Header\n";
    dprintf(fd, "ELF Header\n");

    char mag[5] = { "\0" };
    strncpy(mag, e_ident, 4);
    dprintf(fd, "Magic number: %s\n", mag);

    char * capacity = Elf_describe_ei_class(e_ident[EI_CLASS]);
    dprintf(fd, "Capacity: %s\n", capacity);

    char * endianness = Elf_describe_ei_data(e_ident[EI_DATA]);
    dprintf(fd, "Endianness: %s\n", endianness);
    dprintf(fd, "Version: %d\n", e_ident[EI_VERSION]);

    char * type = Elf_describe_e_type(hdr->e_type);
    dprintf(fd, "Type: %s\n", type);

    char * machine = Elf_describe_e_machine(hdr->e_machine);
    dprintf(fd, "Machine: %s\n", machine);
    dprintf(fd, "Version: %d\n", hdr->e_version);
    dprintf(fd, "Entry: 0x%x\n", hdr->e_entry);
    dprintf(fd, "Program Header Offset: 0x%x\n", hdr->e_phoff);
    dprintf(fd, "Section Header Table Offset: 0x%x\n", hdr->e_shoff);
    dprintf(fd, "Flags: %x\n", hdr->e_flags);
    dprintf(fd, "ELF Header Size: %d bytes\n", hdr->e_ehsize);
    dprintf(fd, "ELF Program File Header Entry Size: %d bytes\n", hdr->e_phentsize);
    dprintf(fd, "ELF Program File No. Entries: %d\n", hdr->e_phnum);
    dprintf(fd, "Section Header Size: %d bytes\n", hdr->e_shentsize);
    dprintf(fd, "Section Header No. Entries: %d\n", hdr->e_shnum);
    dprintf(fd, "Section Name String Table Index: %d\n", hdr->e_shstrndx);
}


void Elf_print_shdr(int fd, Elf32_Shdr * shdr) {
    dprintf(fd, "Section Name: %d\n", shdr->sh_name);
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


char * Elf_describe_ei_data(uint8_t ei_data) {
    switch (ei_data) {
        case ELFDATANONE:
            return "Invalid data encoding";
        case ELFDATA2LSB:
            return "Little-endian";
        case ELFDATA2MSB:
            return "Big-endian";
        default:
            return "Unknown";
    }
}


void Elf_print_error(int fd, char * prefix, Elf_ErrNo errno) {
    dprintf(fd, "%s %d\n", prefix, errno);
}