CC=gcc
CFLAGS=-c -w
LDFLAGS=-ldwarf -lelf 
SOURCES=main.c control.c units.c util.c subprogram.c breakpoint.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=debugger

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf *.o $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@