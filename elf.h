#pragma once

#include "elftype.h"

#define Elf_OK              0
#define Elf_INVALID_HDR     1
#define Elf_INVALID_ARCH    2

typedef uint8_t Elf_ErrNo;

Elf32_Ehdr * Elf_read_elf_header(int fd);
Elf_ErrNo Elf_validate_elf_header(Elf32_Ehdr * elf_hdr);
