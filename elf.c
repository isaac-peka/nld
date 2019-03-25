#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "elf.h"


Elf32_Ehdr * Elf_read_ehdr(int fd, Elf_ErrNo * errptr) {
    Elf_ErrNo errno = Elf_OK;
    Elf32_Ehdr * ehdr = NULL;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    ehdr = malloc(sizeof(Elf32_Ehdr));

    if (ehdr == NULL) {
        errno = Elf_BAD_ALLOC;
        goto CLEANUP;
    }

    if (read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    if (strncmp(ehdr->e_ident, "\x7f""ELF", 4) != 0) {
        errno = Elf_EHDR_BAD_MAG;
        goto CLEANUP;
    }

    // For now we only support x86 32b

    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 &&
        ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {

        errno = Elf_EHDR_BAD_ARCH;
        goto CLEANUP;
    }

    *errptr = errno;
    return ehdr;

    CLEANUP: {
        free(ehdr);
        *errptr = errno;
        return NULL;
    }
}


Elf32_Shdr * Elf_read_shdrs(int fd, Elf32_Ehdr * ehdr, Elf_ErrNo * errptr) {
    Elf_ErrNo errno = Elf_OK;
    Elf32_Shdr * shdrs = NULL;

    Elf32_Off shoff = ehdr->e_shoff;

    if (shoff == 0) {
        errno = Elf_EHDR_BAD_SHOFF;
        goto CLEANUP;
    }

    if (lseek(fd, shoff, SEEK_SET) != shoff) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    if (ehdr->e_shentsize != sizeof(Elf32_Shdr)) {
        errno = Elf_EHDR_BAD_SHENTSIZE;
        goto CLEANUP;
    }

    Elf32_Word shsize = ehdr->e_shnum * ehdr->e_shentsize;
    shdrs = malloc(shsize);

    if (shdrs == NULL) {
        errno = Elf_BAD_ALLOC;
        goto CLEANUP;
    }

    if (read(fd, shdrs, shsize) != shsize) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    *errptr = errno;
    return shdrs;

    CLEANUP: {
        free(shdrs);
        *errptr = errno;
        return NULL;
    }
}


void * Elf_read_section(int fd, Elf32_Shdr * shdr, Elf_ErrNo * errptr) {
    Elf_ErrNo errno = Elf_OK;
    void * buf = malloc(shdr->sh_size);

    if (buf == NULL) {
        errno = Elf_BAD_ALLOC;
        goto CLEANUP;
    }

    if (lseek(fd, shdr->sh_offset, SEEK_SET) == -1) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    if (read(fd, buf, shdr->sh_size) != shdr->sh_size) {
        errno = Elf_BAD_READ;
        goto CLEANUP;
    }

    *errptr = errno;
    return buf;

    CLEANUP: {
        free(buf);
        *errptr = errno;
        return NULL;
    }
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


char * Elf_describe_sh_type(uint8_t sh_type) {
    switch(sh_type) {
        case SHT_NULL:
            return "NULL";
        case SHT_PROGBITS:
            return "PROGBITS";
        case SHT_SYMTAB:
            return "SYMTAB";
        case SHT_DYNSYM:
            return "DYNSYM";
        case SHT_STRTAB:
            return "STRTAB";
        case SHT_RELA:
            return "RELA";
        case SHT_HASH:
            return "HASH";
        case SHT_DYNAMIC:
            return "DYNAMIC";
        case SHT_NOTE:
            return "NOTE";
        case SHT_NOBITS:
            return "NOBITS";
        case SHT_REL:
            return "REL";
        case SHT_SHLIB:
            return "SHLIB";
    }

    if (sh_type >= SHT_LOPROC && sh_type <= SHT_HIPROC) {
        return "Processor-specific";
    }

    if (sh_type >= SHT_LOUSER && sh_type <= SHT_HIUSER) {
        return "Application-specific";
    }

    return "Unknown";
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
    dprintf(fd, "Section Name Index: %d\n", shdr->sh_name);

    char * sh_type = Elf_describe_sh_type(shdr->sh_type);
    dprintf(fd, "Type: %s\n", sh_type);
    dprintf(fd, "Flags: %x\n", shdr->sh_flags);
    dprintf(fd, "Addr: 0x%x\n", shdr->sh_addr);
    dprintf(fd, "Offset: 0x%x\n", shdr->sh_offset);
    dprintf(fd, "Size: %d\n", shdr->sh_size);
    dprintf(fd, "Link: %x\n", shdr->sh_link);
    dprintf(fd, "Info: %x\n", shdr->sh_info);
    dprintf(fd, "Addr-align: %d\n", shdr->sh_addralign);
    dprintf(fd, "Entry size: %x\n", shdr->sh_entsize);
}


void Elf_print_error(int fd, char * prefix, Elf_ErrNo errno) {
    dprintf(fd, "%s %d\n", prefix, errno);
}