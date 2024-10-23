#ifndef CHIP8_H
	#define CHIP8_H 

typedef struct chip8 Chip8;
void init(Chip8* chip8);
void load_rom(Chip8* chip8);
void emulate(Chip8* chip8);

#endif
