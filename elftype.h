#pragma once

#include <stdint.h>

#define EI_NIDENT   16

typedef uint32_t    Elf32_Addr;     // unsigned program address
typedef uint16_t    Elf32_Half;     // unsigned medium integer
typedef uint32_t    Elf32_Off;      // unsigned file offset
typedef int32_t     Elf32_Sword;    // signed large integer
typedef uint32_t    Elf32_Word;     // unsigned large integer

typedef struct {
    uint8_t     e_ident[EI_NIDENT];
    Elf32_Half  e_type;
    Elf32_Half  e_machine;
    Elf32_Word  e_version;
    Elf32_Addr  e_entry;
    Elf32_Off   e_phoff;
    Elf32_Off   e_shoff;
    Elf32_Word  e_flags;
    Elf32_Half  e_ehsize;
    Elf32_Half  e_phentsize;
    Elf32_Half  e_phnum;
    Elf32_Half  e_shentsize;
    Elf32_Half  e_shnum;
    Elf32_Half  e_shstrndx;
} Elf32_Ehdr;

// e_type

#define ET_NONE     0       // no file type
#define ET_REL      1       // relocatable file
#define ET_EXEC     2       // executable file
#define ET_DYN      3       // shared object file
#define ET_CORE     4       // core file
#define ET_LOPROC   0xff00  // processor-specific
#define ET_HIPROC   0xffff  // processor-specific

// e_machine

#define EM_NONE     0
#define EM_M32      1
#define EM_SPARC    2
#define EM_386      3
#define EM_68K      4
#define EM_88K      5
#define EM_860      7
#define EM_MIPS     8

// e_version

#define EV_NONE     0
#define EV_CURRENT  1

// e_ident

#define EI_MAG0     0
#define EI_MAG1     1
#define EI_MAG2     2
#define EI_MAG3     3
#define EI_CLASS    4
#define EI_DATA     5
#define EI_VERSION  6
#define EI_PAD      7

// EI_CLASS

#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2

// EI_DATA

#define ELFDATANONE     0
#define ELFDATA2LSB     1   // 2s compliment little-endian
#define ELFDATA2MSB     2   // 2s compliment big-endian

