/*
MemoryUsage.h - MemoryUsage library
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

// Must be used only one time, outside any function.
#define STACK_DECLARE      byte *stack_end_max;

// Must be used inside a function, as soon as possible, basicaly at the beginning of the setup().
// Can be recalled at any time to reset the stack counter and have a local count of stack bytes.
#define STACK_START        byte b_Stack;   stack_end_max = &b_Stack;

// Must be call to update the current maximum size of the stack, at each function beginning.
#define STACK_COMPUTE      { byte b_Stack;   \
    byte *stack_local_end = &b_Stack;    \
    if ((int) stack_local_end < (int) stack_end_max)\
        stack_end_max = stack_local_end; }

// Compute the current maximum and show it now with customized text.
#define STACK_PRINT(text)  { STACK_COMPUTE; Serial.print(text);  Serial.println(RAMEND - (int) stack_end_max); }

// Compute the current maximum and show it now with default text.
#define STACK_PRINT        STACK_PRINT(F("Stack Size:"));

// Shows the current free SRAM memory with customized text.
#define FREERAM_PRINT(text)  Serial.print(text);  Serial.println(freeRam());

// Shows the current free SRAM memory with default text.
#define FREERAM_PRINT      FREERAM_PRINT(F("Free Ram Size:"));

// Thanks to adafruit : https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#endif
