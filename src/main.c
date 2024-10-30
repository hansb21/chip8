#include <GL/glut.h>
#include "chip8.h"
#include <stdio.h>

Chip8 * chip8;

// Function to update the display array (for demonstration)
void draw(Chip8* chip8) {
	glBegin(GL_QUADS); 

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
		    if (chip8->gfx[x + y * SCREEN_WIDTH] == 1) {
			// Draw a pixel (1) as a small square
			float pixelX = (float)x / SCREEN_WIDTH * 2 - 1; // Normalize x to [-1, 1]
			float pixelY = (float)y / SCREEN_HEIGHT * 2 - 1; // Normalize y to [-1, 1]
			glVertex2f(pixelX, pixelY);
			glVertex2f(pixelX + 2.0f / SCREEN_WIDTH, pixelY); // Right corner
			glVertex2f(pixelX + 2.0f / SCREEN_WIDTH, pixelY + 2.0f / SCREEN_HEIGHT); // Top right corner
			glVertex2f(pixelX, pixelY + 2.0f / SCREEN_HEIGHT); // Top left corner
		    }
        }
    }

    glEnd();
    glFlush();
}

void myInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background color
    gluOrtho2D(-1, 1, -1, 1); // Set the coordinate system
}

void display() {
	if (chip8->V[0xF]) {
		glClear(GL_COLOR_BUFFER_BIT);
		draw(chip8);
		chip8->V[0xF] = 0;	
	}
}
void idle() {
    emulate(chip8);  // Call the emulate function
    glutPostRedisplay(); // Request a redraw
}

int main(int argc, char** argv) {

    chip8 = malloc(sizeof(Chip8)); 
    if (chip8 == NULL) {
	    fprintf(stderr, "failed to allocante memory");
	    return EXIT_FAILURE;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 320); // Scale the window
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chip-8 Emulator Display");
    myInit();
    
	init(chip8);
	load_rom(chip8);



    //updateDisplay(); // Update display for demonstration
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    

    free(chip8);
    return 0;
}

