// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <strings.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <exceptions.h>
#include <idtLoader.h>
#include <scheduler.h>
#include <interrupts.h>
#include <semaphores.h>
#include <pipes.h>
#include <ioManager.h>


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
	load_idt();
	ncClear();

	_cli();
	initSemaphores();
	_sti();

	_cli();
	initPipes();
	_sti();

	_cli();
	initScheduler();
	_sti();

	saveInitialState((uint64_t)sampleCodeModuleAddress, getSP());
	
	((EntryPoint)sampleCodeModuleAddress)();
	/* No vuelve a este punto despues de que se borra firstProcess */
	while(1);
	printWithColor("Game over\n", RED_BLACK);
	
	return 0;
}
