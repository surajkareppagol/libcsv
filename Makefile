CC      = gcc
INCLUDE = $(wildcard include/*.h)
SRCS    = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, build/%.o, $(SRCS))
CFLAGS  = -Wall -std=c11

build: $(OBJECTS) $(wildcard *.c)
	mkdir -p build
	$(CC) $(CFLAGS) -I./include $^ -o build/libcsv

lib: $(OBJECTS)
	mkdir -p lib
	ar rcs lib/libcsv.a $^

$(OBJECTS): build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -I./include $< -o $@

clean:
	rm -f build/*
	rm -f lib/*