CC      = gcc
INCLUDE = $(wildcard include/*.h)
SRCS    = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, build/%.o, $(SRCS))

run: build
	@./build/libcsv

build: $(OBJECTS)
	$(CC) $^ -o build/libcsv

$(OBJECTS): build/%.o: src/%.c
	$(CC) -I./include -c $< -o $@

clean:
	rm -f build/*