build: elf.c test.c utils.c
	gcc -g -Werror -m32 -c -o build/elf.o elf.c
	gcc -g -Werror -m32 -c -o build/test.o test.c
	gcc -g -Werror -m32 -c -o build/utils.o utils.c
	gcc -g -Werror -m32 -o bin/test build/elf.o build/test.o build/utils.o
