CC      = gcc
INCLUDE = $(wildcard include/*.h)
SRCS    = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, build/%.o, $(SRCS))

build: $(OBJECTS) $(wildcard *.c)
	$(CC) -I./include $^ -o build/libcsv

$(OBJECTS): build/%.o: src/%.c
	$(CC) -I./include -c $< -o $@

clean:
	rm -f build/*
	