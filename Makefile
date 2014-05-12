CC=clang
CFLAGS=-c -Wall -Isrc/
LDFLAGS=-ldwarf -lelf 
SOURCES= \
	src/main.c \
	src/line.c \
	src/util.c \
	src/state.c \
	src/command.c \
	src/runtime/general/help.c \
	src/runtime/general/exit.c \
	src/runtime/general/select.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=kvet

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf *.o $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
