#include "keyboard_input.h"
#include <glm/gtc/matrix_transform.hpp>

bool keys[1024];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void processInput(GLFWwindow *window) {
    float cameraSpeed = 2.5f * deltaTime; // Adjust speed as needed
    float rotationSpeed = 50.0f * deltaTime; // Adjust rotation speed as needed

    if (keys[GLFW_KEY_UP])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_DOWN])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_LEFT])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_RIGHT])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // Vertical movement
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraUp;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraUp;

    // Horizontal rotation
    if (keys[GLFW_KEY_D])
        cameraYaw += rotationSpeed;
    if (keys[GLFW_KEY_A])
        cameraYaw -= rotationSpeed;
}