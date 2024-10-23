#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

#define MEM_SIZE 4096
#define NUM_REG 16
#define STACK_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define OPCODE chip8->opcode
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
};

unsigned char key[16];

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

//Chip8 chip8;
void init(Chip8* chip8) 
{

	chip8->pc = 0x200; // PC starts at 0x200
	chip8->opcode = 0; // Reset opcode
	chip8->I = 0;	   // Reset index register
	chip8->sp = 0;	   // Reset Stack Pointer
	
	for (int i = 0; i < MEM_SIZE; i++)
		chip8->memory[i] = 0;

	#ifdef DEBUG
		printf("loading fontset\n");
	#endif

	for (int i = 0; i < 0x50; ++i) // Fonts start at memory location 0x50
		chip8->memory[i+0x50] = chip8_fontset[i];
	
	for (int i = 0; i < NUM_REG; i++)
		chip8->V[i] = 0;

	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
		chip8->gfx[i] = 0;


	chip8->delay_timer = 0;
	chip8->sound_timer = 0;
}

void load_rom(Chip8* chip8) {
    FILE* rom = fopen("Chip8 emulator Logo [Garstyciuks].ch8", "rb");
    if (rom == NULL) {
        printf("Error opening ROM file.\n");
        exit(-1);
    }
    
    if (fseek(rom, 0, SEEK_END) < 0) {
        fclose(rom);
        printf("Error seeking in ROM file.\n");
        exit(-1);
    }
    
    long size = ftell(rom);
    
    if (size > MEM_SIZE - 0x200) {
        fclose(rom);
        printf("ROM too large for memory.\n");
        exit(-1);
    }

    fseek(rom, 0, SEEK_SET); // Reset file pointer to the beginning

    size_t read_size = fread(&chip8->memory[0x200], sizeof(unsigned char), size, rom);
    if (read_size != size) {
        fclose(rom);
        printf("Error reading ROM file.\n");
        exit(-1);
    }

    fclose(rom);
    printf("ROM loaded successfully.\n");
}
void emulate(Chip8* chip8)
{
	OPCODE = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

	printf("0x%X\n", OPCODE & 0xF000);
	switch(chip8->opcode & 0xF000) 
	{
		case 0x1000:
		
			chip8->pc = OPCODE;
		break;

		case 0xA000: //ANNN
		//! TODO 
		
			chip8->I = OPCODE & 0x0FFF;
			chip8->pc += 2;
		break;
		
		case 0x00E0:
			printf("Uknown opcode: 0x%X\n", OPCODE);
			chip8->I = OPCODE & 0x0FFF;
			chip8->pc += 2;

		
		break;

		default:
			printf("Uknown opcode: 0x%X\n", OPCODE);
			
	}
}
