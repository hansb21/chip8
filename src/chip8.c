#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

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
unsigned short *stack[STACK_SIZE];
unsigned short sp; //Stack Pointer

unsigned char *gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

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
		chip8->memory[i] = chip8_fontset[i];
	
	for (int i = 0; i < NUM_REG; i++)
		chip8->V[i] = 0;

	for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
		chip8->gfx[i] = 0;


	chip8->delay_timer = 0;
	chip8->sound_timer = 0;
}

}
