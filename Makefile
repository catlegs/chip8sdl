OBJS = $(OBJDIR)/chip8video.o $(OBJDIR)/chip8.o $(OBJDIR)/main.o
CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall -lSDLmain -lSDL -largtable2
BINDIR = bin
SRCDIR = src
OBJDIR = obj
MAINEXE = $(BINDIR)/chip8sdl


# link the executable
chip8sdl: $(OBJS) $(BINDIR)
	$(CC) $(OBJS) $(LFLAGS) -o $(MAINEXE)


# individual object files
$(OBJDIR)/chip8video.o: $(SRCDIR)/chip8video.c $(SRCDIR)/chip8video.h $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/chip8video.c -o $(OBJDIR)/chip8video.o

$(OBJDIR)/chip8.o: $(SRCDIR)/chip8.c $(SRCDIR)/chip8.h $(SRCDIR)/chip8video.h $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/chip8.c -o $(OBJDIR)/chip8.o

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/chip8.h $(SRCDIR)/chip8video.h $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

# declare clean as a phony target
.PHONY: clean

# clean
clean:
	rm $(OBJS) $(MAINEXE) && rmdir $(OBJDIR) $(BINDIR)