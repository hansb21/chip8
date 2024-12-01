#ifndef CHIP8_H
	#define CHIP8_H 

#define MEM_SIZE 4096
#define NUM_REG 16
#define STACK_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


struct chip8 {

unsigned short opcode; 
unsigned char memory[MEM_SIZE];
unsigned char V[NUM_REG]; // Registers V0-VE 

unsigned short I; // Index Register
unsigned short pc; //Program Counter
unsigned short stack[STACK_SIZE];
unsigned short sp; //Stack Pointer

unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

unsigned char delay_timer;
unsigned char sound_timer;
unsigned char key[16];
};

typedef struct chip8 Chip8;
void init(Chip8* chip8);
void load_rom(Chip8* chip8, const char* filename);
void emulate(Chip8* chip8);

#endif
