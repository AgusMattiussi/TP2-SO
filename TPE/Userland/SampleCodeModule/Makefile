include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c) # ANTES DECIA [^_]*.c
SOURCES_ASM=$(wildcard asm/*.asm) 
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o) 
OBJECTS=$(SOURCES:.c=.o) 

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o *.bin asm/*.o

.PHONY: all clean print
