#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float cameraYaw;
extern float deltaTime;
extern bool keys[];

void processInput(GLFWwindow *window);

#endif