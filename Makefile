DIR = `pwd`
CC  = gcc
CFLAGS = -Wall -Werror -g
LD = gcc
LDFLAGS =
RM = rm -r -f
LIBS =
OBJS =
TARGET = nytwd

BUILDDIR = $(DIR)/build
SRCDIR = $(DIR)/src
TESTDIR = $(DIR)/test

all:	builddir $(TARGET)

builddir:
		mkdir -p $(BUILDDIR)

$(TARGET):	build/nytwd.o build/packet.o build/socket.o
		$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

build/nytwd.o:	src/nytwd.h src/nytwd.c
		$(CC) $(CFLAGS) -c -o $@ src/nytwd.c

build/packet.o:	src/packet.h src/packet.c
		$(CC) $(CFLAGS) -c -o $@ src/packet.c

build/socket.o:	lib/socket.h lib/socket.c
		$(CC) $(CFLAGS) -c -o $@ lib/socket.c

test:

clean:
		$(RM) $(BUILDDIR)
		find $(TESTDIR) -executable -type f -delete
		$(RM) $(TARGET)
