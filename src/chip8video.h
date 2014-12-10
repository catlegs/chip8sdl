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
chip8video.h
    Contains definitions for the struct "chip8video" and functions to interact with it.
*/


#ifndef CHIP8VIDEO_H
#define CHIP8VIDEO_H


#include <SDL/SDL.h>

#define SCALE_FACTOR  (4)
#define SCREEN_WIDTH  (128)
#define SCREEN_HEIGHT (64)

/* argument s is SDL_Surface* screen */
#define COLOR_WHITE (0xFFFFFF)
#define COLOR_BLACK (0x000000)
#define COLOR_RED   (0xFF0000)
#define COLOR_GREEN (0x00FF00)
#define COLOR_BLUE  (0x0000FF)

typedef enum {CHIP8_VID, SCHIP_VID} graphics_mode;

typedef struct {
	graphics_mode mode; /* what the current graphics mode is */
	Uint32 bg_color;
	Uint32 fg_color;
	SDL_Surface* screen;     /* representation of the screen */
	int scale;
} chip8video;



int chip8video_init(chip8video* v, int s, Uint32 bg, Uint32 fg);
int chip8video_clearScreen(chip8video* v);


/* returns -1 on error, 1 if there is a sprite collision, 0 otherwise */
/*   s - pointer to sprite */
/*   n - number of bytes for sprite */
int chip8video_drawSprite(chip8video* v, Uint8 x, Uint8 y, Uint8* s, Uint8 n);
int chip8video_drawExtendedSprite(chip8video* v, Uint8 x, Uint8 y, Uint8* s);
void chip8video_updateScreen(chip8video* v);

/* scrolling */
int chip8video_scrollRight(chip8video* v);
int chip8video_scrollLeft(chip8video* v);
int chip8video_scrollDown(chip8video* v, Uint8 n);

#endif /* CHIP8VIDEO_H */
