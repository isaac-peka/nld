#pragma once

#include "elfspec.h"


/* Errors */


typedef int Elf_ErrNo;

#define Elf_OK                  0
#define Elf_BAD_READ            1
#define Elf_BAD_ALLOC           2
#define Elf_NEED_EHDR           100
#define Elf_NEED_SHDRS          101
#define Elf_NEED_SHDR           102
#define Elf_NEED_SHSTRTAB       103
#define Elf_EHDR_BAD_MAG        200
#define Elf_EHDR_BAD_ARCH       201
#define Elf_EHDR_BAD_SHOFF      202
#define Elf_EHDR_BAD_SHENTSIZE  203

void Elf_print_error(int fd, char * prefix, Elf_ErrNo errno);


/* Headers */


Elf32_Ehdr * Elf_read_ehdr(int fd, Elf_ErrNo *);
Elf32_Shdr * Elf_read_shdrs(int fd, Elf32_Ehdr *, Elf_ErrNo *);
Elf32_Shdr * Elf_read_shstrtab(int fd, Elf32_Ehdr *, Elf_ErrNo *);


/* Sections */


void * Elf_read_section(int fd, Elf32_Shdr *, Elf_ErrNo *);


/* Display utils */


void Elf_print_ehdr(int fd, Elf32_Ehdr *);
void Elf_print_shdr(int fd, Elf32_Shdr *);

char * Elf_describe_e_type(Elf32_Half e_type);
char * Elf_describe_e_machine(Elf32_Half e_machine);
char * Elf_describe_ei_class(uint8_t ei_class);
char * Elf_describe_ei_data(uint8_t ei_data);
