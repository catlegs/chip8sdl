/*
Copyright (c) 2012 Ross Keenum (rosskeenum@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/*
chip8.h
    Contains definitions for the struct "chip8" and functions to interact with it.
*/


#ifndef CHIP8_H
#define CHIP8_H

#include <SDL/SDL.h>
#include "chip8video.h"

typedef struct {
	/* registers */
	Uint8  V[16]; /* general purpose registers */
	Uint16 I;       /* memory addressing register */
	Uint16 PC;      /* program counter */
	Uint8  SP;      /* stack pointer */

	/* timers */
	Uint8 DT; /* delay timer */
	Uint8 ST; /* sound timer */

	/* memory */
	Uint16 stack[16];   /* 16 level deep stack */
	Uint8  mem[0x1000]; /* 4k of memory */

	/* video */
	chip8video vid;

	/* input */
	Uint8 keys[0x10];

	/* hp48 flags (needed for 2 instructions) */
	Uint8 hp48flags[0x8];

    /* how many instructions per second */
    int freq;
} chip8;


int chip8_init(chip8* c, int scale, int freq, Uint32 bg_color, Uint32 fg_color);
int chip8_execute_instruction(chip8* c);

/* load program at file denoted by path */
int chip8_load_program(chip8* c, const char* path);

Uint8 chip8_getKeyPressed(chip8* c);

/* check whether key is pressed or not
   returns zero if not pressed, nonzero if it is */
int chip8_isKeyPressed(chip8* c, Uint8 key);

/* returns the key that is pressed down
   if multiple keys are pressed, return the one with the lowest value
   if no key is pressed, return 0xFF */
Uint8 chip8_getKey(chip8* c);

void chip8_updateKeypresses(chip8* c);

#endif /* CHIP8_H */
