# makefile de PoFlash
PROG = PoFlash

CC = gcc -g
# gcc
CFLAGS =  -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-function -Wno-char-subscripts -Wdeprecated-declarations -O3 -Wall -Wextra -o

LDFLAGS= 
# -DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_2 

OBJECTS =   main.o

PKG_CONFIG = `pkg-config --cflags --libs gtk4` 

install: all

all: $(PROG)

$(PROG): $(OBJECTS)
	$(CC) $(CFLAGS) $(PROG) $(OBJECTS) $(PKG_CONFIG) $(LDFLAGS)

main.o: main.c main.h
	$(CC) $(CFLAGS) main.o -c main.c $(PKG_CONFIG)


.PHONY: clean
clean:
	rm *.o
