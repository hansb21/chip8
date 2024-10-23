#include "chip8.h"

int main(int argc, char *argv[])
{
	 Chip8 * chip8;
	init(chip8);
	load_rom(chip8);

	for (;;){ 
		emulate(chip8);
	}
	return 0;
}
