OBJS = $(OBJDIR)/Chip8Common.o $(OBJDIR)/Instructions.o $(OBJDIR)/Keys.o $(OBJDIR)/Memory.o $(OBJDIR)/Registers.o $(OBJDIR)/Timers.o $(OBJDIR)/Video.o
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

$(OBJDIR)/Instructions.o: $(SRCDIR)/Instructions.cpp $(SRCDIR)/Instructions.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Instructions.cpp -o $(OBJDIR)/Instructions.o

$(OBJDIR)/Keys.o: $(SRCDIR)/Keys.cpp $(SRCDIR)/Keys.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Keys.cpp -o $(OBJDIR)/Keys.o

$(OBJDIR)/Memory.o: $(SRCDIR)/Memory.cpp $(SRCDIR)/Memory.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Memory.cpp -o $(OBJDIR)/Memory.o

$(OBJDIR)/Registers.o: $(SRCDIR)/Registers.cpp $(SRCDIR)/Registers.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Registers.cpp -o $(OBJDIR)/Registers.o

$(OBJDIR)/Timers.o: $(SRCDIR)/Timers.cpp $(SRCDIR)/Timers.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Timers.cpp -o $(OBJDIR)/Timers.o

$(OBJDIR)/Video.o: $(SRCDIR)/Video.cpp $(SRCDIR)/Video.hpp $(OBJDIR)
	$(CC) $(CFLAGS) $(SRCDIR)/Video.cpp -o $(OBJDIR)/Video.o

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
