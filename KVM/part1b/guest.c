#include <stddef.h>
#include <stdint.h>

static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

void HC_print8bit(uint8_t val)
{
	outb(0xE9, val);
}

// outl instruction
void outl_inst(uint32_t given_value, uint16_t given_port)
{
	asm("outl %0,%1" : /* empty */ : "a" (given_value), "Nd" (given_port) : "memory");
}

void HC_print32bit(uint32_t val)
{
	outl_inst(val, 0xE4);
}

uint32_t HC_numExits()
{
	// get the exit value from the hypervisor
	uint32_t get_value_from_hypervisor;
	asm("in %1, %0" : "=a"(get_value_from_hypervisor) : "Nd"(0xE5) : "memory");
	return get_value_from_hypervisor;
}

void HC_printStr(char *str)
{
	uintptr_t y = (uintptr_t)str;
	uint32_t x = (((uint32_t)-1) & y);
	outl_inst(x,0xE2);
}

char *HC_numExitsByType()
{
	uint32_t get_value_from_hypervisor;
	asm("in %1, %0" : "=a"(get_value_from_hypervisor) : "Nd"(0xE3) : "memory");
	return (char *)(uintptr_t)get_value_from_hypervisor;
}

uint32_t HC_gvaToHva(uint32_t gva)
{
	outl_inst(gva, 0xF1);
	uint32_t GPA_from_hypervisor;
	asm("in %1, %0" : "=a"(GPA_from_hypervisor) : "Nd"(0xF1) : "memory");
	return GPA_from_hypervisor;
}

void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;

	for (p = "Hello 695!\n"; *p; ++p)
		HC_print8bit(*p);


	/*----------Don't modify this section. We will use grading script---------*/
	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(2048);
	HC_print32bit(4294967295);

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();

	char *str = "CS695 Assignment 2\n";
	HC_printStr(str);

	num_exits_b = HC_numExits();

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);

	char *firststr = HC_numExitsByType();
	uint32_t hva;
	hva = HC_gvaToHva(1024);
	HC_print32bit(hva);
	hva = HC_gvaToHva(4294967295);
	HC_print32bit(hva);
	char *secondstr = HC_numExitsByType();

	HC_printStr(firststr);
	HC_printStr(secondstr);
	/*------------------------------------------------------------------------*/

	*(long *) 0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
