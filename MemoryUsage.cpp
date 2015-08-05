/*
MemoryUsage.c - MemoryUsage library
Copyright (c) 2015 Thierry Paris.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "MemoryUsage.h"

// Thanks to adafruit : https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int mu_freeRam()
{
	//extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

// Copy / adaptation of the library StackPaint available here : https://github.com/WickedDevice/StackPaint

#define STACK_CANARY	0xc5

void mu_StackPaint(void) __attribute__((naked)) __attribute__((section(".init1")));

void mu_StackPaint(void)
{
#if 1
	uint8_t *p = &_end;

	while (p <= &__stack)
	{
		*p = STACK_CANARY;
		p++;
	}
#else
	__asm volatile (
		"    ldi r30,lo8(_end)\n"
		"    ldi r31,hi8(_end)\n"
		"    ldi r24,lo8(0xc5)\n" // STACK_CANARY = 0xc5
		"    ldi r25,hi8(__stack)\n"
		"    rjmp .cmp\n"
		".loop:\n"
		"    st Z+,r24\n"
		".cmp:\n"
		"    cpi r30,lo8(__stack)\n"
		"    cpc r31,r25\n"
		"    brlo .loop\n"
		"    breq .loop"::);
#endif
}

uint16_t mu_StackCount(void)
{
	uint8_t *p = (__brkval == 0 ? (uint8_t *) &__heap_start : __brkval);

	while (*p == STACK_CANARY && (int) p <= SP)
		p++;

	return (uint16_t)RAMEND - (uint16_t)p;
}
