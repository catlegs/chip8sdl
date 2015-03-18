OBJS = $(OBJDIR)/Chip8Common.o $(OBJDIR)/Chip8Manager.o $(OBJDIR)/Chip8Memory.o $(OBJDIR)/Chip8Timers.o $(OBJDIR)/Chip8Cpu.o
CC = g++
CFLAGS = -Wall -std=c++11 -c
# LFLAGS = -Wall -lSDLmain -lSDL -largtable2
TESTCFLAGS = -Wall -std=c++11 -lpthread -I$(SRCDIR)
BINDIR = bin
SRCDIR = src/main
TESTSRCDIR = src/test
OBJDIR = obj
#MAINEXE = $(BINDIR)/chip8sdl
TESTEXE = $(BINDIR)/chip8tests


# link the executable
#chip8sdl: $(OBJS) $(BINDIR)
#	$(CC) $(OBJS) $(LFLAGS) -o $(MAINEXE)

# build the test executable
$(TESTEXE): $(OBJS) $(BINDIR)
	$(CC) $(TESTSRCDIR)/*.cpp $(OBJS) $(TESTCFLAGS) -o $(TESTEXE)
    
# individual object files
$(OBJDIR)/Chip8Common.o: $(SRCDIR)/Chip8Common.cpp $(SRCDIR)/Chip8Common.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Chip8Common.cpp -o $(OBJDIR)/Chip8common.o

$(OBJDIR)/Chip8Memory.o: $(SRCDIR)/Chip8Memory.cpp $(SRCDIR)/Chip8Memory.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Chip8Memory.cpp -o $(OBJDIR)/Chip8Memory.o

$(OBJDIR)/Chip8Manager.o: $(SRCDIR)/Chip8Manager.cpp $(SRCDIR)/Chip8Manager.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Chip8Manager.cpp -o $(OBJDIR)/Chip8Manager.o
    
$(OBJDIR)/Chip8Timers.o: $(SRCDIR)/Chip8Timers.cpp $(SRCDIR)/Chip8Timers.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Chip8Timers.cpp -o $(OBJDIR)/Chip8Timers.o
    
$(OBJDIR)/Chip8Cpu.o: $(SRCDIR)/Chip8Cpu.cpp $(SRCDIR)/Chip8Cpu.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Chip8Cpu.cpp -o $(OBJDIR)/Chip8Cpu.o

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)


# declare clean as a phony target
.PHONY: clean

# clean
clean:
	rm $(OBJS) $(TESTEXE) && rmdir $(OBJDIR) $(BINDIR)

.PHONY: test

# run the unit tests
test:
	$(TESTEXE)
