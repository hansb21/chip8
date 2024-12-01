#include <GL/glut.h>
#include "chip8.h"
#include <stdio.h>
#include <unistd.h> 

Chip8 * chip8;

// Function to update the display array (for demonstration)
void draw(Chip8* chip8) {
	    float pixelWidth = 2.0f / SCREEN_WIDTH;
	    float pixelHeight = 2.0f / SCREEN_HEIGHT;

	glBegin(GL_QUADS); 

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
		    if (chip8->gfx[x + y * SCREEN_WIDTH] == 1) {
			float pixelX = x * pixelWidth - 1;
			float pixelY = 1 - (y + 1) * pixelHeight;
			glVertex2f(pixelX, pixelY);
			glVertex2f(pixelX + pixelWidth, pixelY);
			glVertex2f(pixelX + pixelWidth, pixelY - pixelHeight);
			glVertex2f(pixelX, pixelY - pixelHeight);

			}
		}
	}

    glEnd();
    glFlush();
    glutSwapBuffers();

}

void myInit(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Background color
    gluOrtho2D(-1, 1, -1, 1); // Set the coordinate system
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	draw(chip8);
	chip8->V[0xF] = 0;	
}
void idle() {
    emulate(chip8);  // Call the emulate function
    glutPostRedisplay(); // Request a redraw
    usleep(300000); 
}

int main(int argc, char** argv) {

    chip8 = malloc(sizeof(Chip8)); 
    if (chip8 == NULL) {
	    fprintf(stderr, "failed to allocante memory");
	    return EXIT_FAILURE;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 320); // Scale the window
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chip-8 Emulator Display");
    myInit();
    
    if (argc < 2) {
	    fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
	    return EXIT_FAILURE;
}
    init(chip8);
    load_rom(chip8, argv[1]);



    //updateDisplay(); // Update display for demonstration
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    

    free(chip8);
    return 0;
}

