#include <GLFW/glfw3.h>
#include <vector>
#include <stack>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void display();

void updateUserInterface();

void reshape(GLFWwindow *window, int width, int height);

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseButton(GLFWwindow* window, int button, int action, int mods);

void cursorPos(GLFWwindow* window, double x, double y);

void charInput (GLFWwindow* window, unsigned int scanChar);

void scroll (GLFWwindow* window, double x, double y);

void destroy();

bool initGlfw();

bool initUserInterface();

int main(void);

