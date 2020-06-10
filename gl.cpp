#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

//constants
const float WINDOW_WIDTH = 600.0f,
WINDOW_HEIGHT = 600.0f;

//Variables
float  camPosX, camPosY;
float camPosZ = camPosY = camPosX = 1.0f;
float angle = 0.0f;

//Functions declaration
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawPyramid();


int main(void) {
    //create GLFW window
    GLFWwindow* window;

    if (!glfwInit()) { // failed to initialize GLFW
        exit(1);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello GLFW ", 0, NULL);

    if (!window) { //failed to create window
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);


    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / float(height);
        glViewport(0, 0, width, height);


        glEnable(GL_DEPTH_TEST); //surface transparency
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-ratio, ratio, -ratio, ratio, 1.0f, 40.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //display
        glTranslated(0.0f, 0.0f, -4.0f);
        gluLookAt(camPosX, camPosY, camPosZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        drawPyramid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void drawPyramid() {
    glBegin(GL_QUADS);
    glColor3f(38.0f / 255, 48.0f / 255, 67.0f / 255);
    glVertex3f(-1, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(173.0f / 255, 12.0f / 255, 57.0f / 255);
    glNormal3f(0, 1, 3);
    glVertex3f(0, 3, 0); //head
    glVertex3f(-1, 0, 1);
    glVertex3f(1, 0, 1);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(38.0f / 255, 64.0f / 255, 96.0f / 255);
    glNormal3f(3, 1, 0);
    glVertex3f(0, 3, 0); //head
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(88.0f / 255, 81.0f / 255, 35.0f / 255);
    glNormal3f(0, 1, -3);
    glVertex3f(0, 3, 0); //head
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(255.0f / 255, 188.0f / 255, 66.0f / 255);
    glNormal3f(-3, 1, 0);
    glVertex3f(0, 3, 0); //head
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 0, 1);
    glEnd();
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_UP: //up
            camPosY += 1;
            break;

        case GLFW_KEY_DOWN: //down
            camPosY -= 1;
            break;

        case GLFW_KEY_RIGHT: //right
            camPosX += 1;
            break;

        case GLFW_KEY_LEFT: // left
            camPosX -= 1;
            break;

        case GLFW_KEY_SPACE: //space
            camPosZ = camPosY = camPosX = 1.0f;
            break;

        case GLFW_KEY_R: //R
            angle += 20;
            break;

        }
    }
}