/*
MemoryUsage.h - MemoryUsage library V2.10
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

#ifndef __MemoryUsage_h__
#define __MemoryUsage_h__

#include <stdint.h>

/*
Roughly, the SRAM memory is divided into four areas: the static data, the heap, the free ram and the stack.
The static data size is given by the compiler itself after the building. this is filled by all variables and 
arrays declared in global scope, or with 'static' keyword.
The heap is filled with all the dynamic allocations done with 'new' keyword or 'malloc' functions.
The stack start from the end of the SRAM area and grow and shrink downward at each function call.

SRAM memory
+---------------+------------------+---------------------------------------------+-----------------+
|               |                  |                                             |                 |
|               |                  |                                             |                 |
|     static    |                  |                                             |                 |
|      data     |       heap       |                    free ram                 |     stack       |
|               |                  |                                             |                 |
|               |                  |                                             |                 |
|               |                  |                                             |                 |
+---------------+------------------+---------------------------------------------+-----------------+
       _end or __heap_start     __brkval                                         SP             RAMEND
       
Source : http://www.nongnu.org/avr-libc/user-manual/malloc.html
*/

extern uint8_t _end;
extern uint8_t __stack;
extern uint8_t *__brkval;
extern uint8_t *__data_start;
extern uint8_t *__data_end;
extern uint8_t *__heap_start;
extern uint8_t *__heap_end;
extern uint8_t *__bss_start;
extern uint8_t *__bss_end;

//
// Memory addresses
//

#define MEMORY_PRINT_START		{ Serial.print(F("Data start:")); Serial.println((int) &__data_start); }
#define MEMORY_PRINT_HEAPSTART	{ Serial.print(F("Heap start:")); Serial.println((int)&__heap_start); }
#define MEMORY_PRINT_HEAPEND	{ Serial.print(F("Heap end:")); Serial.println(__brkval == 0 ? (int)&__heap_start : (int)__brkval); }
#define MEMORY_PRINT_STACKSTART	{ Serial.print(F("Stack start:")); Serial.println((int) SP); }
#define MEMORY_PRINT_END		{ Serial.print(F("Stack end:")); Serial.println((int) RAMEND); }

#define MEMORY_PRINT_HEAPSIZE	{ Serial.print(F("Heap size:")); Serial.println((int) (__brkval == 0 ? (int)&__heap_start : (int)__brkval) - (int)&__heap_start); }
#define MEMORY_PRINT_STACKSIZE	{ Serial.print(F("Stack size:")); Serial.println((int) RAMEND - (int)SP); }
#define MEMORY_PRINT_FREERAM	{ Serial.print(F("Free ram:")); Serial.println((int) SP - (int) (__brkval == 0 ? (int)&__heap_start : (int)__brkval) - (int)&__heap_start); }
#define MEMORY_PRINT_TOTALSIZE	{ Serial.print(F("SRAM size:")); Serial.println((int) RAMEND - (int) &__data_start); }

void SRamDisplay(void);

//
// Stack count part. STACK_COMPUTE will get the maximum size of the stack at the moment...
//

// Must be used only one time, outside any function.
#define STACK_DECLARE      unsigned int mu_stack_size = (RAMEND - SP);

// Must be called to update the current maximum size of the stack, at each function beginning.
#define STACK_COMPUTE      { mu_stack_size = (RAMEND - SP) > mu_stack_size ? (RAMEND - SP) : mu_stack_size;}

// Compute the current maximum and show it now with customized text.
#define STACK_PRINT_TEXT(text)  { STACK_COMPUTE; Serial.print(text);  Serial.println(mu_stack_size); }

// Compute the current maximum and show it now with default text.
#define STACK_PRINT        STACK_PRINT_TEXT(F("Stack Maximum Size (Instrumentation method): "));

//
// Free Ram part.
//

// Shows the current free SRAM memory with customized text.
#define FREERAM_PRINT_TEXT(text)  Serial.print(text);  Serial.println(mu_freeRam());

// Shows the current free SRAM memory with default text.
#define FREERAM_PRINT      FREERAM_PRINT_TEXT(F("Free Ram Size: "));

int mu_freeRam(void);

//
// StackPaint part. This macro gives a view of used stack area at the end of execution...
//

uint16_t mu_StackCount(void);

// Compute the current maximum and show it now with customized text.
#define STACKPAINT_PRINT_TEXT(text)  { Serial.print(text);  Serial.println(mu_StackCount()); }

// Compute the current maximum and show it now with default text.
#define STACKPAINT_PRINT        STACKPAINT_PRINT_TEXT(F("Stack Maximum Size (Painting method): "));


#endif
