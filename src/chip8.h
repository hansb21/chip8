#ifndef CHIP8_H
	#define CHIP8_H 

#define MEM_SIZE 4096
#define NUM_REG 16
#define STACK_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

typedef struct chip8 Chip8;
void init(Chip8* chip8);
void load_rom(Chip8* chip8);
void emulate(Chip8* chip8);

#endif
