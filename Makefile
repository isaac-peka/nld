build:
	gcc -m32 -c -o build/elf.o elf.c
	gcc -m32 -c -o build/test.o test.c
	gcc -m32 -o dist/test build/elf.o build/test.o
