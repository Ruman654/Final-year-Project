/*#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>

// Ball properties
float ballX = -0.5f;
float ballY = 0.0f;
float ballRadius = 0.05f;
float ballXSpeed = 0.0f;
float ballYSpeed = 0.0f;
float gravity = -0.0005f;
float jumpSpeed = 0.015f;

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Game states
enum GameState { START, PLAYING, GAME_OVER };
GameState currentState = START;

// Score
int score = 0;

// Pipe properties
struct Pipe {
    float x;
    float gapY;
    float width;
    float gapHeight;
};

std::vector<Pipe> pipes;
float pipeSpeed = 0.005f;
float pipeSpawnInterval = 2.0f; // Time interval between new pipes
float pipeSpawnTimer = 0.0f;

void drawBall(float x, float y, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * (M_PI / 180.0f);
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
}

void drawPipe(Pipe p) {
    // Draw top pipe
    glBegin(GL_QUADS);
    glVertex2f(p.x, 1.0f);
    glVertex2f(p.x + p.width, 1.0f);
    glVertex2f(p.x + p.width, p.gapY + p.gapHeight / 2.0f);
    glVertex2f(p.x, p.gapY + p.gapHeight / 2.0f);
    glEnd();

    // Draw bottom pipe
    glBegin(GL_QUADS);
    glVertex2f(p.x, -1.0f);
    glVertex2f(p.x + p.width, -1.0f);
    glVertex2f(p.x + p.width, p.gapY - p.gapHeight / 2.0f);
    glVertex2f(p.x, p.gapY - p.gapHeight / 2.0f);
    glEnd();
}

bool checkCollision(float ballX, float ballY, float ballRadius, Pipe p) {
    if (ballX + ballRadius > p.x && ballX - ballRadius < p.x + p.width) {
        if (ballY + ballRadius > p.gapY + p.gapHeight / 2.0f || ballY - ballRadius < p.gapY - p.gapHeight / 2.0f) {
            return true;
        }
    }
    return false;
}

void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentState == START) {
        // Draw start screen
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        drawText("Press SPACE to Start", -0.2f, 0.0f);
    } else if (currentState == PLAYING) {
        // Draw background color
        glColor3f(0.0f, 0.0f, 0.5f); // Dark blue background
        glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        // Draw the ball
        glColor3f(1.0f, 0.0f, 0.0f); // Red color
        drawBall(ballX, ballY, ballRadius);

        // Draw the pipes
        glColor3f(0.0f, 1.0f, 0.0f); // Green color
        for (const Pipe& pipe : pipes) {
            drawPipe(pipe);
        }

        // Draw the score
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        std::string scoreText = "Score: " + std::to_string(score);
        drawText(scoreText.c_str(), -0.95f, 0.9f);
    } else if (currentState == GAME_OVER) {
        // Draw background color
        glColor3f(0.0f, 0.0f, 0.5f); // Dark blue background
        glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        // Draw game over screen
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        drawText("Game Over", -0.2f, 0.1f);
        drawText("Press SPACE to Restart", -0.3f, -0.1f);
        std::string finalScoreText = "Final Score: " + std::to_string(score);
        drawText(finalScoreText.c_str(), -0.3f, 0.0f);
    }

    glutSwapBuffers();
}

void update(int value) {
    if (currentState == PLAYING) {
        // Apply gravity to the ball's Y speed
        ballYSpeed += gravity;

        // Update the ball's position
        ballY += ballYSpeed;
        ballX += ballXSpeed;

        // Update the pipes' positions
        for (Pipe& pipe : pipes) {
            pipe.x -= pipeSpeed;
        }

        // Remove pipes that are off the screen
        pipes.erase(std::remove_if(pipes.begin(), pipes.end(), [](Pipe p) { return p.x + p.width < -1.0f; }), pipes.end());

        // Spawn new pipes at intervals
        pipeSpawnTimer += pipeSpeed;
        if (pipeSpawnTimer >= pipeSpawnInterval) {
            pipeSpawnTimer = 0.0f;
            float gapY = (rand() % 200 - 100) / 100.0f; // Random gap position
            pipes.push_back({1.0f, gapY, 0.1f, 0.4f});
            score++;
        }

        // Check for collisions
        for (const Pipe& pipe : pipes) {
            if (checkCollision(ballX, ballY, ballRadius, pipe)) {
                currentState = GAME_OVER;
            }
        }

        // Check for game over condition (ball falls off the screen)
        if (ballY - ballRadius < -1.0f || ballY + ballRadius > 1.0f) {
            currentState = GAME_OVER;
        }
    }

    // Redisplay the updated scene
    glutPostRedisplay();

    // Call update function again after 16 milliseconds (approximately 60 FPS)
    glutTimerFunc(16, update, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void handleKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        if (currentState == PLAYING) {
            ballYSpeed = jumpSpeed; // Make the ball jump
        }
    } else if (key == GLUT_KEY_LEFT) {
        if (currentState == PLAYING) {
            ballXSpeed = -0.01f; // Move the ball left
        }
    } else if (key == GLUT_KEY_RIGHT) {
        if (currentState == PLAYING) {
            ballXSpeed = 0.01f; // Move the ball right
        }
    } else if (key == GLUT_KEY_DOWN) {
        if (currentState == PLAYING) {
            ballYSpeed = -0.01f; // Move the ball down
        }
    }
}

void handleSpecialKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        ballXSpeed = 0.0f; // Stop horizontal movement
    } else if (key == GLUT_KEY_DOWN) {
        ballYSpeed = 0.0f; // Stop downward movement
    }
}

void handleKeys(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (currentState == START) {
            currentState = PLAYING;
            score = 0;
            pipes.clear();
            ballX = -0.5f;
            ballY = 0.0f;
            ballXSpeed = 0.0f;
            ballYSpeed = 0.0f;
        } else if (currentState == GAME_OVER) {
            currentState = START;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Floating Ball Game with Pipe Barriers");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutSpecialFunc(handleKeys);
    glutSpecialUpFunc(handleSpecialKeysUp);
    glutKeyboardFunc(handleKeys);

    glClearColor(0.0f, 0.0f, 0.5f, 1.0f); // Dark blue background

    glutMainLoop();
    return 0;
}
