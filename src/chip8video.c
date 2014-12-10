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
chip8video.c
    Contains implementation of functions declared in "chip8video.h"
*/


#include "chip8video.h"
#include <stdlib.h>

/* returns 0 on success, otherwise error */
int chip8video_init(chip8video* v, int s, Uint32 bg, Uint32 fg)
{
    /* initialize video */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    /* register SDL to quit at exit of program */
    atexit(SDL_Quit);

    /* create the screen */
    v->scale = s;
    if((v->screen = SDL_SetVideoMode(SCREEN_WIDTH * s, SCREEN_HEIGHT * s, 32, SDL_SWSURFACE)) == NULL)
        return 2;

    v->mode = CHIP8_VID;
    v->bg_color = SDL_MapRGB(v->screen->format, (Uint8) (bg >> 8) & 0xFF, (Uint8) (bg >> 4) & 0xFF, (Uint8) bg & 0xFF);
    v->fg_color = SDL_MapRGB(v->screen->format, (Uint8) (fg >> 8) & 0xFF, (Uint8) (fg >> 4) & 0xFF, (Uint8) fg & 0xFF);

    if(chip8video_clearScreen(v) > 0)
        return 1;

    return 0;
}


/* returns -1 on error, 0 on success */
int chip8video_clearScreen(chip8video* v)
{
    /* lock screen */
    if(SDL_MUSTLOCK(v->screen))
        if(SDL_LockSurface(v->screen) < 0)
            return -1;

    {
        Uint32* pixel;
        int x,y;
        int wid;
         for(y = 0; y < v->screen->h; ++y)
        {
            wid = v->screen->pitch / 4;
            pixel = (Uint32*)v->screen->pixels + (y * wid);
            for(x = 0; x < wid; ++x, ++pixel)
            {
                *pixel = v->bg_color;
            }
        }
    }

    if(SDL_MUSTLOCK(v->screen))
        SDL_UnlockSurface(v->screen);

    SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);

    return 0;
}

/* returns -1 on error, 1 if there is a sprite collision, 0 otherwise */
/*   s - pointer to sprite */
/*   n - number of bytes for sprite (if 0, draw 16x16 sprite (8x8 in CHIP8 mode) */
int chip8video_drawSprite(chip8video* v, Uint8 x, Uint8 y, Uint8* s, Uint8 n)
{
    int scale = v->scale;
    int collision = 0;
    int sprite_iswrapped = 0;

    /* lock screen */
    if(SDL_MUSTLOCK(v->screen))
        if(SDL_LockSurface(v->screen) < 0)
            return -1;

        Uint32* pixel;
        Uint32  pixel_color;
        int i, j;
        int i2, j2;
        int spriteWidth;
        int h = SCREEN_HEIGHT;
        int w = SCREEN_WIDTH;
        int x_add;
        int y_mul;


        /* set correct sprite width */
        if(n == 0)
        {
            spriteWidth = 16;
            n = 16;
            y_mul = 2;
        }
        else
        {
            spriteWidth = 8;
            y_mul = 1;
        }

        /* if in CHIP8 mode then the scale is twice what it would be in SCHIP mode */
        if(v->mode == CHIP8_VID)
        {
            scale <<= 1;
            w >>= 1;
            h >>= 1;
            spriteWidth = 8;
            y_mul = 1;

            x %= SCREEN_WIDTH / 2;
            y %= SCREEN_WIDTH / 2;
        }
        else
        {
            x %= SCREEN_WIDTH;
            y %= SCREEN_WIDTH;
        }

            for(j = 0; j < n; ++j)
            {
                x_add = 0;
                for(i = 0; i < spriteWidth; ++i)
                {

                    if(i >= 8)
                        x_add = 1;

                    /* implement sprite wrapping */
                    if(((y+j) >= h) || ((x+i) >= w))
                        sprite_iswrapped = 1;

                    pixel = (Uint32*)v->screen->pixels + ( (((y+j) % h) * scale) * (v->screen->pitch / 4) + (((x+i) % w) * scale) );
                    if((0x80 >> (i % 8)) & s[j * y_mul + x_add])
                    {
                        if((*pixel) == (v->fg_color))
                        {
                            collision = 1;
                            pixel_color = v->bg_color;
                        }
                        else
                            pixel_color = v->fg_color;
                        for(j2 = 0; j2 < scale; ++j2)
                        {
                            pixel = (Uint32*)v->screen->pixels + ( ((((y+j) % h) * scale) + j2) * (v->screen->pitch / 4) + (((x+i) % w) * scale) );
                            for(i2 = 0; i2 < scale; ++i2, ++pixel)
                                *pixel = pixel_color;

                        }
                    }
                }
            }


    /* unlock screen */
    if(SDL_MUSTLOCK(v->screen))
        SDL_UnlockSurface(v->screen);

    /* if this sprite is wrapped, update the entire screen */
    if(sprite_iswrapped)
        SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);
    else
        SDL_UpdateRect(v->screen, x * scale, y * scale, spriteWidth * scale, n * scale);

    return collision;
}



/* scrolling */

/* scroll 4 pixels right (SCHIP) 2 right (CHIP8) */
int chip8video_scrollRight(chip8video* v)
{
    /* lock screen */
    if(SDL_MUSTLOCK(v->screen))
        if(SDL_LockSurface(v->screen) < 0)
            return -1;

    Uint32* pix_orig;
    Uint32* pix_new;
    int i, j;
    int yofs;

    for(j=0, yofs=0; j < v->screen->h; ++j, yofs += v->screen->pitch / 4)
    {
        pix_new = (Uint32*) v->screen->pixels + (v->screen->w - 1) + yofs;
        pix_orig = (Uint32*) pix_new - (v->scale * 4);

        /* if the pix_orig has went off the left edge of the screen, set pix_new to the background color */
        for(i=0; i < v->screen->w; ++i, --pix_new, --pix_orig)
            if((v->screen->w - i) < (v->scale * 4))
                *pix_new = v->bg_color;
            else
                *pix_new = *pix_orig;
    }


    /* unlock screen */
    if(SDL_MUSTLOCK(v->screen))
        SDL_UnlockSurface(v->screen);

    SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);
    return 0;
}


/* scroll 4 pixels left (SCHIP) 2 left (CHIP8) */
int chip8video_scrollLeft(chip8video* v)
{
    /* lock screen */
    if(SDL_MUSTLOCK(v->screen))
        if(SDL_LockSurface(v->screen) < 0)
            return -1;

    Uint32* pix_orig;
    Uint32* pix_new;
    int i, j;
    int yofs;

    for(j=0, yofs=0; j < v->screen->h; ++j, yofs += v->screen->pitch / 4)
    {
        pix_new = (Uint32*) v->screen->pixels + yofs;
        pix_orig = (Uint32*) pix_new + (v->scale * 4);

        /* if the pix_orig has went off the right edge of the screen, set pix_new to the background color */
        for(i=0; i < v->screen->w; ++i, ++pix_new, ++pix_orig)
            if((v->screen->w - i) <= (v->scale * 4))
                *pix_new = v->bg_color;
            else
                *pix_new = *pix_orig;
    }


    /* unlock screen */
    if(SDL_MUSTLOCK(v->screen))
        SDL_UnlockSurface(v->screen);

    SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);
    return 0;
}


int chip8video_scrollDown(chip8video* v, Uint8 n)
{
    /* lock screen */
    if(SDL_MUSTLOCK(v->screen))
        if(SDL_LockSurface(v->screen) < 0)
            return -1;

    Uint32* pix_orig;
    Uint32* pix_new;
    int i, j;
    int yofs;

    for(j=0, yofs = (v->screen->pitch / 4) * (v->screen->h - 1); j < v->screen->h; ++j, yofs -= v->screen->pitch / 4)
    {
        pix_new = (Uint32*) v->screen->pixels + yofs;
        pix_orig = (Uint32*) pix_new - (v->scale * n * (v->screen->pitch / 4) );

        /* if the pix_orig has went off the right edge of the screen, set pix_new to the background color */
        for(i=0; i < v->screen->w; ++i, ++pix_new, ++pix_orig)
            if(pix_orig < ((Uint32*)v->screen->pixels))
                *pix_new = v->bg_color;
            else
                *pix_new = *pix_orig;
    }


    /* unlock screen */
    if(SDL_MUSTLOCK(v->screen))
        SDL_UnlockSurface(v->screen);

    SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);
    return 0;
}

void chip8video_updateScreen(chip8video* v)
{
    SDL_UpdateRect(v->screen, 0, 0, v->screen->w, v->screen->h);
}
