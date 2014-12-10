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
main.c
    Contains the client implementation.
*/


#include <stdio.h>
#include <SDL/SDL.h>
#include <argtable2.h>
#include <string.h>
#include "chip8.h"

/* default values for command line arguments */
#define DEFAULT_INSTR_PER_SEC (800)
#define DEFAULT_SCALE_FACTOR  (4)
#define DEFAULT_BG_COLOR      (0x000000)
#define DEFAULT_FG_COLOR      (0xFFFFFF)

#define WINDOW_NAME "CHIP-8 SDL"
#define PROG_NAME   "chip8sdl"
#define VERSION     "0.1b"

/* subtract 3 from timers each 50ms */
#define TIMER_DELAY (50)

/* if return is not NULL, there were errors and the error string is the return value */
int parseArguments(chip8* c, int argc, char** argv);

int main(int argc, char** argv)
{
	chip8 c;

	Uint32 timer_now;
	Uint32 timer_next;

	/* timers for instruction execution */
	Uint32 inst_timer_now;
	Uint32 inst_timer_next;

	int inst_per_5ms;
	int inst_executed;

	if(parseArguments(&c, argc, argv) > 0)
	    return 1;

    SDL_WM_SetCaption(WINDOW_NAME, NULL);
    inst_per_5ms =  c.freq / 200;

    timer_now = SDL_GetTicks();
    timer_next = timer_now + TIMER_DELAY;

    inst_timer_now = SDL_GetTicks();
    inst_timer_next = inst_timer_now + 5;


	for(;;)
	{


	    for(inst_executed=0; inst_executed < inst_per_5ms; ++inst_executed)
	    {
            /* update keypresses before each instruction. */
            chip8_updateKeypresses(&c);

            /* run an instruction, on failure exit the program */
            if(chip8_execute_instruction(&c))
                exit(1);



            if(timer_next < (timer_now = SDL_GetTicks()))
            {
                /* set the next time to update the timers */
                timer_next += TIMER_DELAY;

                /* update timers */
                if(c.DT < 3)
                    c.DT = 0;
                else
                    c.DT -= 3;

                if(c.ST < 3)
                    c.ST = 0;
                else
                    c.ST -= 3;
            }
	    }

	    if((inst_timer_now = SDL_GetTicks()) < inst_timer_next)
	    {
            SDL_Delay(inst_timer_next - inst_timer_now);
            //printf("sleep %dms\n", inst_timer_next-inst_timer_now);
	    }
        inst_timer_next += 5;

	}
	return 0;
}



/* if return is not NULL, there were errors and the error string is the return value */
int parseArguments(chip8* c, int argc, char** argv)
{
    /* set up the argtable */
    struct arg_int* scale      =  arg_int0(NULL, "scale",      "<int>", "set how much to scale the video (default 4)");
    struct arg_int* freq       =  arg_int0(NULL, "freq",       "<int>", "set how many instructions per second (default 800)");
    struct arg_int* background =  arg_int0(NULL, "background", "<int>", "set background color in format 0xRRGGBB (default 0x000000)");
    struct arg_int* foreground =  arg_int0(NULL, "foreground", "<int>", "set foreground color in format 0xRRGGBB (default 0xFFFFFF)");
    struct arg_lit* help       =  arg_lit0(NULL, "help",           "print this help and exit");
    struct arg_file* file      = arg_file0(NULL, NULL, NULL,           "input file (game)");
    struct arg_end* end        = arg_end(5);

    void* argtable[] = {scale, freq, background, foreground, help, file, end};
    int numErrors;
    char* progname = PROG_NAME;
    /* verify entries were allocated */
    if(arg_nullcheck(argtable) != 0)
        /* some allocations have failed */
        return 1;

    /* set default values */
    scale->ival[0]      = DEFAULT_SCALE_FACTOR;
    freq->ival[0]       = DEFAULT_INSTR_PER_SEC;
    background->ival[0] = DEFAULT_BG_COLOR;
    foreground->ival[0] = DEFAULT_FG_COLOR;

    /* parse command line */
    numErrors = arg_parse(argc, argv, argtable);

    /* if --help was specified */
    if(help->count > 0)
    {
       /* print version and basic usage */

        printf("usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n\n");
        arg_print_glossary(stdout,argtable,"  %-19s %s\n");
        printf("\n%s %s by Ross Keenum\n", progname, VERSION);
        return 1;
    }

    /* if there were errors, print them */
    if(numErrors > 0)
    {
        arg_print_errors(stdout, end, progname);
        printf("Use '%s --help' for more information.\n", progname);
        return 1;
    }

    /* if no command line options were specified */
    if(argc == 1)
    {
        printf("Use '%s --help' for more information.\n", progname);
        return 1;
    }

    /* make sure an input file is given */
    if(file->count == 0)
    {
        printf("No input file specified\n");
        return 1;
    }

    /* take the command line options and initialize c */
    chip8_init(c, scale->ival[0], freq->ival[0], background->ival[0], foreground->ival[0]);
    chip8_load_program(c, file->filename[0]);


    /* deallocate non-null entries in argtable */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));


    return 0;
}
