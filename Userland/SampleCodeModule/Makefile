include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c) # ANTES DECIA [^_]*.c
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o) 
OBJECTS=$(SOURCES:.c=.o) 
OBJECTS_C=$(SOURCES_C:.c=.o) 

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o *.bin asm/*.o c/*.o

.PHONY: all clean print
