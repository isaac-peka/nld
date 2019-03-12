#pragma once

#include "elfspec.h"


/* Errors */


typedef uint16_t Elf_ErrNo;

#define Elf_OK                  0
#define Elf_BAD_READ            1
#define Elf_BAD_ALLOC           2
#define Elf_NEED_EHDR           100
#define Elf_NEED_SHDRS          101
#define Elf_NEED_SHDR           102
#define Elf_EHDR_BAD_MAG        200
#define Elf_EHDR_BAD_ARCH       201
#define Elf_EHDR_BAD_SHOFF      202
#define Elf_EHDR_BAD_SHENTSIZE  203

void Elf_print_error(int fd, char * prefix, Elf_ErrNo errno);


/* Combined state */


typedef struct {
    int             s_fd;
    Elf32_Ehdr *    s_ehdr;
    Elf32_Shdr *    s_shdrs;
} Elf_State;


Elf_State * Elf_open(char * pathname);
void Elf_free_state(Elf_State * state);


/* Headers */


Elf_ErrNo Elf_read_ehdr(Elf_State *);
Elf_ErrNo Elf_read_shdrs(Elf_State *);
Elf_ErrNo Elf_read_string_table(Elf_State *);


/* Display utils */


void Elf_print_ehdr(int fd, Elf32_Ehdr *);
void Elf_print_shdr(int fd, Elf32_Shdr *);

char * Elf_describe_e_type(Elf32_Half e_type);
char * Elf_describe_e_machine(Elf32_Half e_machine);
char * Elf_describe_ei_class(uint8_t ei_class);
char * Elf_describe_ei_data(uint8_t ei_data);
