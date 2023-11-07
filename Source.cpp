#include <glut.h>
#include <cmath>
#include <windows.h>
#include <string>
#include <algorithm>

// Constants for window dimensions
const int window_width = 800;
const int window_height = 600;

// Arrow position and rotation variables
float arrow_x = -window_width / 4.0f;  // Set arrow_x to the center-left
float arrow_y = 0.0f;
float arrow_angle = 0.0f;

// Target position variables
float target_x = window_width / 4.0f;  // Set target_x to the center-right
float target_y = 0.0f;
float target_velocity = 1.0f;  // Adjust the velocity as needed
float target_direction = 1.0f; // Start moving down

// Points for each target color
int greenPoints = 0;
int redPoints = 0;
int bluePoints = 0;

int totalPoints = 0;

bool arrowHasHitTarget = false;

float rectHeights[] = { 60.0f, 40.0f, 30.0f, 40.0f, 60.0f };
float pos_y[] = { 0.0f, 0.0f , 0.0f , 0.0f , 0.0f };

int level = 1;
int maxArrows = 1; // Maximum arrows allowed in each level

// Target points and bow max for each level
int targetPoints[] = { 0, 15, 30, 90, 105, 120 };
int bowMax[] = { 0, 1, 2, 2, 3, 4 };

bool hasShotArrow = false;

// Function to handle key presses
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 27) // Escape key
        exit(0);
}

void drawTarget() {
    // Define the colors for the target rectangles
    float colors[][3] = {
        {1.0f, 0.0f, 0.0f}, // Red
        {0.0f, 0.0f, 1.0f}, // Blue
        {0.0f, 1.0f, 0.0f}, // Green
        {0.0f, 0.0f, 1.0f}, // Blue
        {1.0f, 0.0f, 0.0f}  // Red
    };

    // Heights of the target rectangles
    float rectWidth = 20.0f;

    // Calculate the maximum and minimum Y positions for the target
    float maxY = window_height / 2 - rectHeights[0] / 2;
    float minY = -window_height / 2 + rectHeights[4] / 2;

    // Update the target's vertical position
    target_y += target_velocity * target_direction;

    // Starting Y position for the top rectangle
    float startY = target_y - rectHeights[0] / 2;

    for (int i = 0; i < 5; i++) {
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        float rectX = target_x - rectWidth / 2.0f; // Right-center horizontally

        glBegin(GL_QUADS);
        glVertex2f(rectX, startY);
        glVertex2f(rectX + rectWidth, startY);
        pos_y[i] = startY + rectHeights[i];
        glVertex2f(rectX + rectWidth, pos_y[i]);
        glVertex2f(rectX, pos_y[i]);
        glEnd();

        // Update the starting Y position for the next rectangle
        startY += rectHeights[i];
        if (target_direction == 1 && startY >= maxY) {
            target_direction = -1;
        }
        else if (target_direction == -1 && startY <= minY) {
            target_direction = 1;
        }
    }
}

// Function to display the current level, maximum arrows allowed, and target points
void displayLevelInfo() {
    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 20);
    std::string levelText = "Level - " + std::to_string(level);
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    for (char c : levelText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 40);
    std::string maxArrowsText = "Bow max: " + std::to_string(bowMax[level]);
    for (char c : maxArrowsText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 60);
    std::string targetPointsText = "Target: " + std::to_string(targetPoints[level]);
    for (char c : targetPointsText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

// Function to draw the bow and arrow
void drawBow() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw the arrow
    glColor3f(0.8f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(arrow_x, arrow_y, 0.0f);
    glRotatef(arrow_angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-40.0f, 2.0f);  // Reversed X-coordinate
    glVertex2f(-40.0f, -2.0f); // Reversed X-coordinate
    glEnd();
    glPopMatrix();

    // Draw the target
    drawTarget();

    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 120);
    std::string arrowsText = "Remaining Arrows: " + std::to_string(maxArrows);
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    for (char c : arrowsText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Display level, maximum arrows allowed, and target points
    displayLevelInfo();
    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 100);
    totalPoints = greenPoints + redPoints + bluePoints;
    const char* text3 = "Total Points: ";
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    for (int i = 0; i <= 13; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text3[i]);
    }

    // Display the total points
    std::string totalPointsText = std::to_string(totalPoints);
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    for (char c : totalPointsText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    // Set the text position for displaying points
    glRasterPos2f(-window_width / 2 + 10, window_height / 2 - 80);
    const char* text2 = "Target Point: ";
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color to white
    for (int i = 0; i <= 13; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text2[i]);
    }

    // Display the points for each target color
    std::string greenScore = std::to_string(greenPoints);
    std::string redScore = std::to_string(redPoints);
    std::string blueScore = std::to_string(bluePoints);

    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2f(-window_width / 2 + 110, window_height / 2 - 80);
    for (char c : greenScore) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');  // Add a space
    glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
    glRasterPos2f(-window_width / 2 + 130, window_height / 2 - 80);
    for (char c : redScore) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');  // Add a space
    glColor3f(0.0f, 0.0f, 1.0f); // Set color to blue
    glRasterPos2f(-window_width / 2 + 150, window_height / 2 - 80);
    for (char c : blueScore) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glFlush();
}

// Function to handle arrow key presses
void handleArrowKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        arrow_x -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        arrow_x += 5.0f;
        break;
    case GLUT_KEY_UP:
        arrow_angle += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        arrow_angle -= 5.0f;
        break;
    }
    glutPostRedisplay();
}

// Define the timer callback function
void timerCallback(int value) {
    float radians = arrow_angle * 3.14 / 180.0f; // Convert degrees to radians
    float step = 5.0f; // Adjust this value as needed

    if (arrow_x >= -window_width / 2 && arrow_x <= window_width / 2 &&
        arrow_y >= -window_height / 2 && arrow_y <= window_height / 2) {
        arrow_x += step * cos(radians);
        arrow_y += step * sin(radians);

        // Check for collisions with the target if the arrow has not hit a target in this pass
        if (!arrowHasHitTarget) {
            int rectIndex = (int)((arrow_y + window_height / 2) / (window_height / 5));
            if (rectIndex >= 0 && rectIndex < 5) {
                float rectX = target_x - 10.0f; // Center of the rectangle

                // Calculate the position of the arrow relative to the target's center
                float relativeX = arrow_x - target_x;

                // Check which color of target rectangle was hit based on the relative position
                if (fabs(relativeX) <= 10.0f) {
                    // Arrow is inside the target width-wise
                    if (relativeX > -10.0f && relativeX <= 10.0f) {
                        if (arrow_y >= pos_y[0] && arrow_y <= pos_y[1]) {
                            // Arrow hit the top (blue) rectangle
                            redPoints += 15;
                        }
                        else if (arrow_y >= pos_y[1] && arrow_y <= pos_y[2]) {
                            // Arrow hit the second (red) rectangle
                            bluePoints += 40;
                        }
                        else if (arrow_y >= pos_y[2] && arrow_y <= pos_y[3]) {
                            // Arrow hit the center (green) rectangle
                            greenPoints += 50;
                        }
                        else if (arrow_y >= pos_y[3] && arrow_y <= pos_y[4]) {
                            // Arrow hit the bottom (blue) rectangle
                            bluePoints += 40;
                        }
                        else if (arrow_y >= pos_y[4] && arrow_y <= (pos_y[4] >= 0) ? pos_y[4] - 20 : 20 - pos_y[4]) {
                            // Arrow hit the bottom (blue) rectangle
                            redPoints += 15;
                        }
                        arrowHasHitTarget = true;  // Set the flag to true to prevent further points in this pass
                        int tot = bluePoints + redPoints + greenPoints;
                        if (maxArrows == 0) {
                            
                            if (tot == targetPoints[level]) {
                                level++;
                                bluePoints = 0;
                                redPoints = 0;
                                maxArrows = bowMax[level];
                                greenPoints = 0;
                            }
                            else {
                                level=1;
                                bluePoints = 0;
                                redPoints = 0;
                                maxArrows = bowMax[level];
                                greenPoints = 0;
                            }
                        }
                        if (tot > targetPoints[level]) {
                            level = 1;
                            bluePoints = 0;
                            redPoints = 0;
                            maxArrows = bowMax[level];
                            greenPoints = 0;
                        }
                       
                        
                        //totalPoints!=targetPoints[level] && maxArrows==0 ||
                        if (level == 6 ) {
                            level = 1;
                            maxArrows = bowMax[level];
                            bluePoints = 0;
                            redPoints = 0;
                            greenPoints = 0;
                        }
                        //maxArrows = bowMax[level];
                    }
                }
            }
        }

        glutPostRedisplay();
        glutTimerFunc(20, timerCallback, 0);
    }
    else {
        // Reset the flag when the arrow is out of bounds
        arrowHasHitTarget = false;
        // Reset the arrow's position
        arrow_x = -window_width / 4.0f;
        arrow_y = 0.0f;
        arrow_angle = 0.0f;
    }
}

void handleSpacebarPress(unsigned char key, int x, int y) {
    if (key == 32) { // 32 is the ASCII code for spacebar
        if (maxArrows > 0) {
            // Start the timer callback function
            arrowHasHitTarget = false;
            glutTimerFunc(20, timerCallback, 0);
            maxArrows--; // Reduce the maximum allowed arrows
            hasShotArrow = true;
        }
    }
}

// Function to rotate the bow continuously
void rotateBow(int value) {
    static bool rotateClockwise = false; // Start by rotating counterclockwise

    if (rotateClockwise) {
        arrow_angle += 2.0f; // Rotate clockwise
        if (arrow_angle >= 45.0f) {
            rotateClockwise = false;
        }
    }
    else {
        arrow_angle -= 2.0f; // Rotate counterclockwise
        if (arrow_angle <= -45.0f) {
            rotateClockwise = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, rotateBow, 0); // Call the function again after 20 milliseconds
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Bow and Arrow");
    glutDisplayFunc(drawBow);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleArrowKeys);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-window_width / 2, window_width / 2, -window_height / 2, window_height / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glutKeyboardFunc(handleSpacebarPress);
    maxArrows = bowMax[level]; // Initialize max arrows based on the current level
    // Start the continuous rotation
    glutTimerFunc(20, rotateBow, 0);

    glutMainLoop();

    return 0;
}
