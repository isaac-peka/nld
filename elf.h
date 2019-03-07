#pragma once

#include "elftype.h"

#define Elf_OK              0
#define Elf_INVALID_MAG     1
#define Elf_INVALID_ARCH    2

typedef uint8_t Elf_ErrNo;

Elf32_Ehdr * Elf_read_elf_header(int fd);
Elf_ErrNo Elf_validate_elf_header(Elf32_Ehdr * elf_hdr);
char * Elf_describe_header(Elf32_Ehdr * hdr);
char * Elf_describe_e_type(Elf32_Half e_type);
char * Elf_describe_e_machine(Elf32_Half e_machine);
char * Elf_describe_ei_class(uint8_t ei_class);
char * Elf_describe_ei_data(uint8_t ei_data);
