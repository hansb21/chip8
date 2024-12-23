#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

#define OPCODE chip8->opcode


unsigned    char chip8_fontset[80] =
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

void load_rom(Chip8* chip8, const char* filename) {
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL) {
        printf("Error opening ROM file: %s\n", filename);
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
	chip8->pc += 2;
	unsigned short NNN = OPCODE & 0x0FFF;
	unsigned short N = OPCODE & 0x000F;
	unsigned short KK = OPCODE & 0x00FF;
	unsigned short X = (OPCODE >> 8) & 0x000F;
	unsigned short Y = (OPCODE >> 4) & 0x00FF;

	#ifdef DEBUG
		printf("[DEBUG] Executing opcode: 0x%X at PC: 0x%X\n", OPCODE, chip8->pc);
		printf("OPCODE = 0x%X\n", OPCODE);
		printf(" NNN   = 0x%X\n ", NNN); 
		printf(" N     = 0x%X\n ", N); 
		printf(" KK    = 0x%X\n",KK); 
		printf(" X     = 0x%X\n", X); 
		printf(" Y     = 0x%X\n", Y); 
	#endif

	//printf("0x%X\n", OPCODE & 0xF000);
	switch(OPCODE & 0xF000) 
	{
		case 0x0000:
			switch (KK) 
			{
				case 0x00E0:
					
					printf("0x00E0\n");
					for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++){
						chip8->gfx[i] = 0;
					}

					chip8->V[0xF] = 1;
				break;
			}
		break;
		case 0x1000:
		
			chip8->pc = NNN;
			printf("0x%X\n", chip8->pc);
			printf("0x%X\n", OPCODE);
		break;

		case 0x6000:
			chip8->V[X] = KK;

		break;
		
		case 0x7000:
			chip8->V[X] += KK;

		break;

		case 0xA000: //ANNN
		
			chip8->I = NNN;
		//	chip8->pc += 2;
		break;
		
		case 0xD000:
			chip8->V[0xF] = 0;

			for (int i = 0; i < N; i++) {
				int spriteRow = chip8->memory[chip8->I + i];
				
				for (int j = 0; j < 8; j++) {
					if (spriteRow & (0x80 >> j)) {
						int pixelX = (chip8->V[X] + j) % SCREEN_WIDTH;
						int pixelY = (chip8->V[Y] + i) % SCREEN_HEIGHT;

						int index = pixelX + pixelY * (SCREEN_WIDTH);

						
						if (chip8->gfx[index]) 
							chip8->V[0xF] = 1;

						chip8->gfx[index] ^= 1;
					}

				}
			}
		
		break;

		default:
			printf("Uknown opcode: 0x%X\n", OPCODE);
			
	}


if (chip8->delay_timer > 0)
	--chip8->delay_timer;

if (chip8->sound_timer > 0)
{
	if (chip8->sound_timer == 1)
		printf("BEEP\n");

	--chip8->sound_timer;
}
}
