/* #include <GL/glut.h>
#include <cmath>
#include <iostream>

float ballX = 200.0f;
float ballY = 200.0f;
float ballRadius = 20.0f;
float ballSpeedX = 0.0f; // Horizontal speed
float ballSpeedY = 0.0f; // Vertical speed
int windowWidth = 800;
int windowHeight = 600;
float pipeWidth = 40.0f;
float gapHeight = 150.0f;
float pipeDistance = 300.0f;
float floatAmplitude = 50.0f; // Amplitude of the floating motion
float floatFrequency = 0.005f; // Frequency of the floating motion

void init() {
    glClearColor(0.7, 0.7, 1.0, 1.0); // Light blue background
    gluOrtho2D(0, windowWidth, 0, windowHeight); // Set orthographic projection
}

void drawPipe(float x, float gapY, bool fromTop) {
    glBegin(GL_QUADS);
    if (fromTop) {
        // Top pipe
        glVertex2f(x, gapY - gapHeight);
        glVertex2f(x + pipeWidth, gapY - gapHeight);
        glVertex2f(x + pipeWidth, windowHeight);
        glVertex2f(x, windowHeight);
    } else {
        // Bottom pipe
        glVertex2f(x, 0.0f);
        glVertex2f(x + pipeWidth, 0.0f);
        glVertex2f(x + pipeWidth, gapY + gapHeight);
        glVertex2f(x, gapY + gapHeight);
    }
    glEnd();
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Draw pipes with gaps
    glColor3f(0.5, 0.5, 0.5); // Grey pipes

    // Calculate the number of pipes needed to cover the screen width
    int numPipes = ceil((float)windowWidth / pipeDistance) + 1;

    // Calculate the starting position for pipes based on ballX position
    float startPipeX = fmod(ballX, pipeDistance) - pipeDistance;

    // Loop to draw multiple pipes
    for (int i = 0; i < numPipes; ++i) {
        // Top pipe
        drawPipe(startPipeX + i * pipeDistance, windowHeight, true);
        // Bottom pipe
        drawPipe(startPipeX + i * pipeDistance, 0.0f, false);
    }

    // Update ball position (move horizontally)
    ballX += ballSpeedX;
    if (ballX < -ballRadius) {
        ballX = windowWidth + ballRadius; // Wrap around to the right side
    }
    if (ballX > windowWidth + ballRadius) {
        ballX = -ballRadius; // Wrap around to the left side
    }

    // Update ball position (move vertically - simulate floating)
    ballY = windowHeight / 2 + floatAmplitude * sin(glutGet(GLUT_ELAPSED_TIME) * floatFrequency);

    // Draw the ball
    glColor3f(1.0, 0.0, 0.0); // Red ball
    glBegin(GL_TRIANGLE_FAN);
    for (float angle = 0.0f; angle <= 360.0f; angle += 1.0f) {
        float x = ballX + ballRadius * cos(angle * 3.14159f / 180.0f);
        float y = ballY + ballRadius * sin(angle * 3.14159f / 180.0f);
        glVertex2f(x, y);
    }
    glEnd();

    glFlush(); // Force execution of OpenGL commands
}

void update(int value) {
    glutPostRedisplay(); // Notify GLUT that the display has changed
    glutTimerFunc(16, update, 0); // Call update function in 16 milliseconds
}

void keyPressed(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Escape key
            exit(0); // Exit the program
            break;
    }
}

void specialKeyPressed(int key, int x, int y) {
    // Arrow keys affect both horizontal and vertical movement
    switch (key) {
        case GLUT_KEY_LEFT:
            ballSpeedX = -5.0f; // Move the ball left
            break;
        case GLUT_KEY_RIGHT:
            ballSpeedX = 5.0f; // Move the ball right
            break;
        case GLUT_KEY_UP:
            ballSpeedY = 5.0f; // Move the ball up
            break;
        case GLUT_KEY_DOWN:
            ballSpeedY = -5.0f; // Move the ball down
            break;
    }
}

void specialKeyReleased(int key, int x, int y) {
    // Arrow keys affect both horizontal and vertical movement
    switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            ballSpeedX = 0.0f; // Stop horizontal movement
            break;
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            ballSpeedY = 0.0f; // Stop vertical movement
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(windowWidth, windowHeight); // Set window size
    glutInitWindowPosition(100, 100); // Set window position
    glutCreateWindow("Floating Ball Animation"); // Create the window
    init(); // Initialize OpenGL parameters
    glutDisplayFunc(drawScene); // Set the display callback function
    glutTimerFunc(0, update, 0); // Initialize update function
    glutKeyboardFunc(keyPressed); // Set the keyboard callback function
    glutSpecialFunc(specialKeyPressed); // Set the special key callback function
    glutSpecialUpFunc(specialKeyReleased); // Set the special key release callback function
    glutMainLoop(); // Enter the GLUT event processing loop
    return 0;
}
