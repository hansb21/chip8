CC = gcc

CFLAGS = -Wall -Wextra -I./src

SRCS = src/chip8.c src/main.c

OUTPUT = chip8

all: $(OUTPUT)

$(OUTPUT): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRCS)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean




