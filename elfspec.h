#pragma once

#include <stdint.h>


typedef uint32_t    Elf32_Addr;     // unsigned program address
typedef uint16_t    Elf32_Half;     // unsigned medium integer
typedef uint32_t    Elf32_Off;      // unsigned file offset
typedef int32_t     Elf32_Sword;    // signed large integer
typedef uint32_t    Elf32_Word;     // unsigned large integer


/* ELF Header */


#define EI_NIDENT   16

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


/* ELF Sections */


#define SHN_UNDEF       0       // undefined, missing, irrelevant
#define SHN_LORESERVE   0xff00  // lower bound of reserved indexes
#define SHN_HIRESERVE   0xffff  // upper bound of reserved indexes
#define SHN_LOPROC      0xff00  // lower bound of processor-specific semantics
#define SHN_HIPROC      0xff1f  // upper bound of processor-specific semantics
#define SHN_ABS         0xff1f  // specifies absolute values for the corresponding reference
#define SHN_COMMON      0xfff2  // symbols defined relativive to this section are common symbols

typedef struct {
    Elf32_Word      sh_name;
    Elf32_Word      sh_type;
    Elf32_Word      sh_flags;
    Elf32_Addr      sh_addr;
    Elf32_Off       sh_offset;
    Elf32_Word      sh_size;
    Elf32_Word      sh_link;
    Elf32_Word      sh_info;
    Elf32_Word      sh_addralign;
    Elf32_Word      sh_entsize;
} Elf32_Shdr;

// sh_type

#define SHT_NULL        0           // section header inactive
#define SHT_PROGBITS    1           // holds information for the program
#define SHT_SYMTAB      2           // holds symbols
#define SHT_STRTAB      3           // holds string table
#define SHT_RELA        4           // holds reloc entries with explicit addends
#define SHT_HASH        5           // holds a symbol hash table
#define SHT_DYNAMIC     6           // holds info for dynamic linking
#define SHT_NOTE        7           // holds info that marks the file in some way
#define SHT_NOBITS      8           // holds no space in file, but otherwise resembles SHT_PROGBITS
#define SHT_REL         9           // holds reloc entries without explcit addends
#define SHT_SHLIB       10          // reserved but has unspecified semantics
#define SHT_DYNSYM      11          // holds dynamic symbols
#define SHT_LOPROC      0x70000000  // lower bound reserved for processor-specific semantics
#define SHT_HIPROC      0x7fffffff  // upper bound reserved for processor-specific semantics
#define SHT_LOUSER      0x80000000  // lower bound reserved for application programs
#define SHT_HIUSER      0xffffffff  // upper bound reserved for application programs

// sh_flags

#define SHF_WRITE       0x1         // contains data that should be writable during program execution
#define SHF_ALLOC       0x2         // occupies memory during process execution
#define SHF_EXECINSTR   0x4         // contains executable machine instructions
#define SHF_MASKPROC    0xf0000000  // mask for reserved bits for processor-specific semantics

// symbol table

typedef struct {
    Elf32_Word      st_name;
    Elf32_Addr      st_value;
    Elf32_Word      st_size;
    uint8_t         st_info;
    uint8_t         st_other;
    Elf32_Half      st_shndx;
} Elf32_Sym;


// st_info

#define ELF32_ST_BIND(i)        ((i) >> 4)
#define ELF32_ST_TYPE(i)        ((i) & 0xf)
#define ELF32_ST_INFO(b, t)     (((b) << 4)) + ((t) & 0xf)

// st_bind

#define STB_LOCAL   0   // local symbols not visible outside file
#define STB_GLOBAL  1   // global symbols visible to all files
#define STB_WEAK    2   // global symbols, with lower precedence definition
#define STB_LOPROC  13  // lower bound for processor-specific semantics
#define STB_HIPROC  15  // upper bound for processor-specific semantics

// st_type

#define STT_NOTYPE      0
#define STT_OBJECT      1
#define STT_FUNC        2
#define STT_SECTION     3
#define STT_FILE        4
#define STT_LOPROC      13
#define STT_HIPROC      15


/* Relocation */


typedef struct {
    Elf32_Addr      r_offset;
    Elf32_Word      r_info;
} Elf32_Rel;


typedef struct {
    Elf32_Addr      r_offset;
    Elf32_Word      r_info;
    Elf32_Sword     r_addend;
} Elf32_Rela;

// r_info

#define ELF32_R_SYM(i)      ((i) >> 8)
#define ELF32_R_TYPE(i)     ((uint8_t)(i))
#define ELF32_R_INFO(s, t)  (((s) << 8) + (uint8_t)(t))

// relocation types

#define R_386_NONE      0
#define R_386_32        1
#define R_386_PC32      2
#define R_386_GOT32     3   // computes the distance from GOT base to GOT entry
#define R_386_PLT32     4   // compute PLT table entry
#define R_386_COPY      5   // dynamic linking, offset for writable secton
#define R_386_GLOB_DAT  6   // set GOT entry to address of symbol
#define R_386_JMP_SLOT  7   // dynamic linking, offset for PLT
#define R_386_RELATIVE  8   // dynamic linking, location within SO for rel address
#define R_386_GOTOFF    9   // compute diff between symbol value and GOT address
#define R_386_GOTPC     10  // similar to R_386_PC32, except use GOT in calculation