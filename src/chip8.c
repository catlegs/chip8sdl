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
chip8.c
    Contains implementation of functions declared in "chip8.h"
*/


#include "chip8.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* for cross platform support */
#ifdef WINDOWS
#include <process.h>
#else
#include <unistd.h>
#endif

#define CHIP8K_0 SDLK_x
#define CHIP8K_1 SDLK_1
#define CHIP8K_2 SDLK_2
#define CHIP8K_3 SDLK_3
#define CHIP8K_4 SDLK_q
#define CHIP8K_5 SDLK_w
#define CHIP8K_6 SDLK_e
#define CHIP8K_7 SDLK_a
#define CHIP8K_8 SDLK_s
#define CHIP8K_9 SDLK_d
#define CHIP8K_A SDLK_z
#define CHIP8K_B SDLK_c
#define CHIP8K_C SDLK_4
#define CHIP8K_D SDLK_r
#define CHIP8K_E SDLK_f
#define CHIP8K_F SDLK_v


int chip8_init(chip8* c, int scale, int freq, Uint32 bg_color, Uint32 fg_color)
{

	/* initialize the registers and memory */
	{
		/* zero out the main memory, stack, and general purpose registers */
		memset(c->mem, 0, sizeof(c->mem));
		memset(c->stack, 0, sizeof(c->stack));
		memset(c->V, 0, sizeof(c->V));
		memset(c->keys, 0, sizeof(c->keys));

		/* initialize other registers */
		c->I = 0;
		c->PC = 0x200;
		c->SP = 0;
		c->DT = 0;
		c->ST = 0;

        c->freq = freq;
        /* initialize video and timers */
		chip8video_init(&(c->vid), scale, bg_color, fg_color);
		SDL_InitSubSystem(SDL_INIT_TIMER);
	}

	/* load fonts */
	{

	    /* set up arrays of fonts */
	    Uint8 chip8_font[0x50] = {0xF0, 0x90, 0x90, 0x90, 0xF0,  /* 0 */
	                           0x20, 0x60, 0x20, 0x20, 0x70,  /* 1 */
	                           0xF0, 0x10, 0xF0, 0x80, 0xF0,  /* 2 */
	                           0xF0, 0x10, 0xF0, 0x10, 0xF0,  /* 3 */
	                           0x90, 0x90, 0xF0, 0x10, 0x10,  /* 4 */
	                           0xF0, 0x80, 0xF0, 0x10, 0xF0,  /* 5 */
	                           0xF0, 0x80, 0xF0, 0x90, 0xF0,  /* 6 */
	                           0xF0, 0x10, 0x20, 0x40, 0x40,  /* 7 */
	                           0xF0, 0x90, 0xF0, 0x90, 0xF0,  /* 8 */
	                           0xF0, 0x90, 0xF0, 0x10, 0xF0,  /* 9 */
	                           0xF0, 0x90, 0xF0, 0x90, 0x90,  /* A */
	                           0xE0, 0x90, 0xE0, 0x90, 0xE0,  /* B */
	                           0xF0, 0x80, 0x80, 0x80, 0xF0,  /* C */
	                           0xE0, 0x90, 0x90, 0x90, 0xE0,  /* D */
	                           0xF0, 0x80, 0xF0, 0x80, 0xF0,  /* E */
	                           0xF0, 0x80, 0xF0, 0x80, 0x80}; /* F */

        Uint8 schip_font[0x100] = {0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38,  /* 0 */
                                0x10, 0x30, 0x50, 0x90, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFE,  /* 1 */
                                0x38, 0x44, 0x82, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFE,  /* 2 */
                                0xFE, 0x02, 0x04, 0x08, 0x18, 0x04, 0x02, 0x02, 0x84, 0x78,  /* 3 */
                                0x04, 0x0C, 0x14, 0x24, 0x44, 0x84, 0xFE, 0x04, 0x04, 0x04,  /* 4 */
                                0xFE, 0x80, 0x80, 0x80, 0xF8, 0x04, 0x02, 0x02, 0x84, 0x78,  /* 5 */
                                0x3C, 0x40, 0x80, 0x80, 0xB8, 0xC4, 0x82, 0x82, 0x44, 0x38,  /* 6 */
                                0xFE, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10,  /* 7 */
                                0x38, 0x44, 0x82, 0x44, 0x38, 0x44, 0x82, 0x82, 0x44, 0x38,  /* 8 */
                                0x38, 0x44, 0x82, 0x82, 0x46, 0x3A, 0x02, 0x02, 0x04, 0x78,  /* 9 */
                                0x10, 0x28, 0x28, 0x28, 0x44, 0x7C, 0x44, 0x82, 0x82, 0x82,  /* A */
                                0xF8, 0x84, 0x82, 0x84, 0xF8, 0x84, 0x82, 0x82, 0x84, 0xF8,  /* B */
                                0x3C, 0x42, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x42, 0x3C,  /* C */
                                0xF8, 0x84, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x84, 0xF8,  /* D */
                                0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80, 0xFE,  /* E */
                                0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80}; /* F */

        /* copy font arrays to memory */
        memcpy(c->mem, chip8_font, sizeof(chip8_font)); /* chip8 font goes at 0x000 through 0x04F */
        memcpy(&(c->mem[0x100]), schip_font, sizeof(schip_font)); /* schip font goes at 0x100 through 0x19F */
	}
	/* seed the random number generator */
	srand((unsigned int)SDL_GetTicks() ^ getpid());

	return 0;
}

/* load program at file denoted by path */
int chip8_load_program(chip8* c, const char* path)
{
    FILE* gamefile = fopen(path, "rb");
    if(gamefile == NULL)
        return 1;
    {
        /* get filesize */
        int filesize;
        fseek(gamefile, 0L, SEEK_END);
        filesize = ftell(gamefile);
        fseek(gamefile, 0L, SEEK_SET);

        if(filesize != fread(&(c->mem[0x200]), sizeof(Uint8), filesize, gamefile))
            return 1;
    }

    return 0;
}


/* execute the instruction where PC is pointing
    returns 0 - success
    returns 1 - failure */
int chip8_execute_instruction(chip8* c)
{
	Uint16 op;
	Uint8 msn;

    /* read the instruction */
	op =  (Uint16)(c->mem[c->PC] << 8);
	op |= (Uint16)(c->mem[c->PC + 1]);

	c->PC += 2; /* increment the program counter */

	msn = op >> 12; /* most significant nibble */
	switch(msn)
	{

	case 0x0:
		{
			/* least significant byte */
			Uint8 lsb = op & 0xFF;
			switch(lsb)
			{
            case 0xFB: /* SCR (SCHIP) */
                chip8video_scrollRight(&(c->vid));
                break;
            case 0xFC: /* SCL (SCHIP) */
                chip8video_scrollLeft(&(c->vid));
                break;
            case 0xFD: /* QUIT (SCHIP) */
                exit(0);
                break;
            case 0xFE: /* LOW  (CHIP-8) */
                c->vid.mode = CHIP8_VID;
                break;
            case 0xFF: /* HIGH (SCHIP) */
                c->vid.mode = SCHIP_VID;
                break;

			case 0xE0: /* CLS */
				chip8video_clearScreen(&(c->vid));
				break;

			case 0xEE: /* RET */
				c->PC = c->stack[c->SP];
				--(c->SP);
				break;

			default:
                if((lsb & 0xF0) == 0xC0) /* SCD nibble */
                {
                    chip8video_scrollDown(&(c->vid), lsb & 0xF);
                    break;
                }
                else /* undefined opcode */
                    return 1;
			}
		}
		break;

	case 0x1: /* JP addr */
		c->PC = op & 0xFFF;
		break;

	case 0x2: /* CALL addr */
		++(c->SP);
		c->stack[c->SP] = c->PC;
		c->PC = op & 0xFFF;
		break;

	case 0x3: /* SE Vx, byte */
		if(c->V[(op & 0xF00) >> 8] == (op & 0xFF))
			c->PC += 2;
		break;

	case 0x4: /* SNE Vx, byte */
		if(c->V[(op & 0xF00) >> 8] != (op & 0xFF))
			c->PC += 2;
		break;

	case 0x5: /* SE Vx, Vy */
		if((op & 0xF) == 0)
		{
			if(c->V[(op & 0xF00) >> 8] == c->V[(op & 0xF0) >> 4])
				c->PC += 2;
		}
		else
			return 1; /* undefined opcode */
		break;

	case 0x6: /* LD Vx, byte */
		c->V[(op & 0xF00) >> 8] = op & 0xFF;
		break;

	case 0x7: /* ADD Vx, byte */
		c->V[(op & 0xF00) >> 8] += op & 0xFF;
		break;

	case 0x8:
		{
			Uint8 lsn = op & 0xF; /* least significant nibble */
			switch(lsn)
			{
			case 0x0: /* LD Vx, Vy */
				c->V[(op & 0xF00) >> 8] = c->V[(op & 0xF0) >> 4];
				break;

			case 0x1: /* OR Vx, Vy */
				c->V[(op & 0xF00) >> 8] |= c->V[(op & 0xF0) >> 4];
				break;

			case 0x2: /* AND Vx, Vy */
				c->V[(op & 0xF00) >> 8] &= c->V[(op & 0xF0) >> 4];
				break;

			case 0x3: /* XOR Vx, Vy */
				c->V[(op & 0xF00) >> 8] ^= c->V[(op & 0xF0) >> 4];
				break;

			case 0x4: /* ADD Vx, Vy */
				{
					Uint16 res = (Uint16)c->V[(op & 0xF00) >> 8] + (Uint16)c->V[(op & 0xF0) >> 4];
					c->V[(op & 0xF00) >> 8] = (Uint8)(res & 0xFF);

					/* check to see whether or not to set the carry flag */
					if(res > 0xFF)
						c->V[0xF] = 1;
					else
						c->V[0xF] = 0;
				}
				break;

			case 0x5: /* SUB Vx, Vy */
				/* set or unset the carry flag */
				if(c->V[(op & 0xF00) >> 8] >= c->V[(op & 0xF0) >> 4])
					c->V[0xF] = 1;
				else
					c->V[0xF] = 0;

				c->V[(op & 0xF00) >> 8] -= c->V[(op & 0xF0) >> 4];
				break;

			case 0x6: /* SHR Vx */
				c->V[0xF] = c->V[(op & 0xF00) >> 8] & 1;
				c->V[(op & 0xF00) >> 8] >>= 1;
				break;

			case 0x7: /* SUBN Vx, Vy */
				/* set or unset the carry flag */
				if(c->V[(op & 0xF0) >> 4] >= c->V[(op & 0xF00) >> 8])
					c->V[0xF] = 1;
				else
					c->V[0xF] = 0;

				c->V[(op & 0xF00) >> 8] = c->V[(op & 0xF0) >> 4] - c->V[(op & 0xF00) >> 8];
				break;

			case 0xE: /* SHL Vx */
				if(c->V[(op & 0xF00) >> 8] & 0x80)
					c->V[0xF] = 1;
				else
					c->V[0xF] = 0;

				c->V[(op & 0xF00) >> 8] <<= 1;
				break;

			default: /* undefined opcode */
				return 1;
			};
		}
		break;

	case 0x9: /* SNE Vx, Vy */
		if((op & 0xF) == 0)
		{
			if(c->V[(op & 0xF00) >> 8] != c->V[(op & 0xF0) >> 4])
				c->PC += 2;
		}
		else
			return 1; /* undefined opcode */
		break;

	case 0xA: /* LD I, addr */
		c->I = op & 0xFFF;
		break;

	case 0xB: /* JP V0, addr */
		c->PC = ((op & 0xFFF) + (Uint16)c->V[0]) & 0xFFF;
		break;

	case 0xC: /* RND Vx, byte */
        c->V[(op & 0xF00) >> 8] = (Uint8)((op & 0xFF) & rand());
		break;

	case 0xD: /* DRW Vx, Vy, nibble */
        c->V[0xF] = chip8video_drawSprite(&(c->vid), c->V[(op & 0xF00) >> 8], c->V[(op & 0xF0) >> 4], &(c->mem[c->I]), op & 0xF);
		break;

	case 0xE:
		{
			/* least significant byte */
			Uint8 lsb = op & 0xFF;

			switch(lsb)
			{
			case 0x9E: /* SKP Vx */
                if(chip8_isKeyPressed(c, c->V[(op & 0xF00) >> 8]))
                    c->PC += 2;
				break;

			case 0xA1: /* SKNP Vx */
                if(!chip8_isKeyPressed(c, c->V[(op & 0xF00) >> 8]))
                    c->PC += 2;
				break;

			default: /* undefined opcode */
				return 1;
			};
		}
		break;

	case 0xF:
		{
			/* least significant byte */
			Uint8 lsb = op & 0xFF;

			switch(lsb)
			{
			case 0x07: /* LD Vx, DT */
				c->V[(op & 0xF00) >> 8] = c->DT;
				break;

			case 0x0A: /* LD Vx, K */
                /* if a key isn't pressed, decrement the PC so we can repeat
                     this instruction until there is one */
				if((c->V[(op & 0xF00) >> 8] = chip8_getKey(c)) == 0xFF)
                    c->PC -= 2;
				break;

			case 0x15: /* LD DT, Vx */
				c->DT = c->V[(op & 0xF00) >> 8];
				break;

			case 0x18: /* LD ST, Vx */
				c->ST = c->V[(op & 0xF00) >> 8];
				break;

			case 0x1E: /* ADD I, Vx */
				c->I += c->V[(op & 0xF00) >> 8];
				break;

			case 0x29: /* LD F, Vx */
				c->I = c->V[(op & 0xF00) >> 8] * 5;
				break;

            case 0x30: /* LD HF, Vx (SCHIP) */
                c->I = 0x100 + (c->V[(op & 0xF00) >> 8] * 10);
                break;

			case 0x33: /* LD B, Vx */
				{
					Uint8 num;
					num = c->V[(op & 0xF00) >> 8];

					c->mem[c->I] = num / 100;
					c->mem[c->I + 1] = (num % 100) / 10;
					c->mem[c->I + 2] = num % 10;
				}
				break;

			case 0x55: /* LD [I], Vx */
				memcpy(&(c->mem[c->I]), c->V, ((op & 0xF00) >> 8) + 1);
				break;

			case 0x65: /* LD Vx, [I] */
				memcpy(c->V, &(c->mem[c->I]), ((op & 0xF00) >> 8) + 1);
				break;

            case 0x75: /* LD R, Vx (SCHIP) */
                memcpy(c->hp48flags, c->V, ((op & 0xF00) >> 8) + 1);
                break;

            case 0x85: /* LD Vx, R (SCHIP) */
                memcpy(c->V, c->hp48flags, ((op & 0xF00) >> 8) + 1);
                break;

			default: /* undefined opcode */
				return 1;
			};
		}
		break;
	};

	/* if we made it through the switch statement we succeeded! */
	return 0;
}



/* check whether key is pressed or not
   returns zero if not pressed, nonzero if it is */
int chip8_isKeyPressed(chip8* c, Uint8 key)
{
    return c->keys[key];
}

/* returns the key that is pressed down
   if multiple keys are pressed, return the one with the lowest value
   if no key is pressed, return 0xFF */
Uint8 chip8_getKey(chip8* c)
{
    int i;
    for(i=0; i < 0x10; ++i)
        if(c->keys[i])
            return i;
    return 0xFF;
}

void chip8_updateKeypresses(chip8* c)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case CHIP8K_0:
                        c->keys[0x0] = 1;
                        break;
                    case CHIP8K_1:
                        c->keys[0x1] = 1;
                        break;
                    case CHIP8K_2:
                        c->keys[0x2] = 1;
                        break;
                    case CHIP8K_3:
                        c->keys[0x3] = 1;
                        break;
                    case CHIP8K_4:
                        c->keys[0x4] = 1;
                        break;
                    case CHIP8K_5:
                        c->keys[0x5] = 1;
                        break;
                    case CHIP8K_6:
                        c->keys[0x6] = 1;
                        break;
                    case CHIP8K_7:
                        c->keys[0x7] = 1;
                        break;
                    case CHIP8K_8:
                        c->keys[0x8] = 1;
                        break;
                    case CHIP8K_9:
                        c->keys[0x9] = 1;
                        break;
                    case CHIP8K_A:
                        c->keys[0xA] = 1;
                        break;
                    case CHIP8K_B:
                        c->keys[0xB] = 1;
                        break;
                    case CHIP8K_C:
                        c->keys[0xC] = 1;
                        break;
                    case CHIP8K_D:
                        c->keys[0xD] = 1;
                        break;
                    case CHIP8K_E:
                        c->keys[0xE] = 1;
                        break;
                    case CHIP8K_F:
                        c->keys[0xF] = 1;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    /* exit to quit the program */
                        exit(0);
                    case CHIP8K_0:
                        c->keys[0x0] = 0;
                        break;
                    case CHIP8K_1:
                        c->keys[0x1] = 0;
                        break;
                    case CHIP8K_2:
                        c->keys[0x2] = 0;
                        break;
                    case CHIP8K_3:
                        c->keys[0x3] = 0;
                        break;
                    case CHIP8K_4:
                        c->keys[0x4] = 0;
                        break;
                    case CHIP8K_5:
                        c->keys[0x5] = 0;
                        break;
                    case CHIP8K_6:
                        c->keys[0x6] = 0;
                        break;
                    case CHIP8K_7:
                        c->keys[0x7] = 0;
                        break;
                    case CHIP8K_8:
                        c->keys[0x8] = 0;
                        break;
                    case CHIP8K_9:
                        c->keys[0x9] = 0;
                        break;
                    case CHIP8K_A:
                        c->keys[0xA] = 0;
                        break;
                    case CHIP8K_B:
                        c->keys[0xB] = 0;
                        break;
                    case CHIP8K_C:
                        c->keys[0xC] = 0;
                        break;
                    case CHIP8K_D:
                        c->keys[0xD] = 0;
                        break;
                    case CHIP8K_E:
                        c->keys[0xE] = 0;
                        break;
                    case CHIP8K_F:
                        c->keys[0xF] = 0;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                exit(0);

            default:
                break;
        }
    }
}
