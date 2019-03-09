#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "elf.h"
#include "utils.h"


Elf32_Ehdr * Elf_read_elf_header(int fd) {
    if (lseek(fd, 0, SEEK_SET) == -1) {
        return NULL;
    }

    Elf32_Ehdr * elf_hdr = calloc(1, sizeof(Elf32_Ehdr));
    if (elf_hdr == NULL) {
        return NULL;
    }

    if (read(fd, elf_hdr, sizeof(elf_hdr)) == -1) {
        free(elf_hdr);
        return NULL;
    }

    return elf_hdr;
}


Elf_ErrNo Elf_validate_elf_header(Elf32_Ehdr * elf_hdr) {
    char * e_ident = elf_hdr->e_ident;

    if (strncmp(e_ident, "\x7f""ELF", 4) != 0) {
        return Elf_INVALID_MAG;
    }

    // for now we only support x86 32

    if (e_ident[EI_CLASS] != ELFCLASS32 &&
            e_ident[EI_DATA] != ELFDATA2LSB) {

        return Elf_INVALID_ARCH;
    }

    return Elf_OK;
}


char * Elf_describe_header(Elf32_Ehdr * hdr) {
    char * e_ident = hdr->e_ident;

    char header[] = "ELF Header\n";
    char * dest = malloc(sizeof(header));
    safe_strcatp(&dest, header);

    char mag[5] = { "\0" };
    strncpy(mag, e_ident, strlen(mag));
    append_asprintf(&dest, "Magic number: %s\n", mag);

    char * capacity = Elf_describe_ei_class(e_ident[EI_CLASS]);
    append_asprintf(&dest, "Capacity: %s\n", capacity);

    char * endianness = Elf_describe_ei_data(e_ident[EI_DATA]);
    append_asprintf(&dest, "Endianness: %s\n", endianness);

    append_asprintf(&dest, "Version: %s\n", e_ident[EI_VERSION]);

    char * type = Elf_describe_e_type(hdr->e_type);
    append_asprintf(&dest, "Type: %s\n", type);

    char * machine = Elf_describe_e_machine(hdr->e_machine);
    append_asprintf(&dest, "Machine: %s\n", machine);

    append_asprintf(&dest, "Version: %d\n", hdr->e_version);
    append_asprintf(&dest, "Entry: 0x%x\n", hdr->e_entry);
    append_asprintf(&dest, "Program Header Offset: 0x%x\n", hdr->e_phoff);
    append_asprintf(&dest, "Section Header Table Offset: 0x%x\n", hdr->e_shoff);
    append_asprintf(&dest, "Flags: %x\n", hdr->e_flags);
    append_asprintf(&dest, "ELF Size: %d bytes\n", hdr->e_ehsize);
    append_asprintf(&dest, "ELF Program File Header Entry Size: %d bytes\n", hdr->e_phentsize);
    append_asprintf(&dest, "ELF Program File No. Entries: %d\n", hdr->e_phnum);
    append_asprintf(&dest, "Section Header Size: %d bytes\n", hdr->e_shentsize);
    append_asprintf(&dest, "Section Header No. Entries: %d\n", hdr->e_shnum);
    append_asprintf(&dest, "Section Name String Table Index: %d\n", hdr->e_shstrndx);

    return dest;
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
