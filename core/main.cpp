#define GL_SILENCE_DEPRECATION

// Without this gl.h gets included instead of gl3.h
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// For includes related to OpenGL, make sure their are included after glfw3.h
#include <OpenGL/gl3.h>

// Utils
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cassert>

// Glm maths library
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local includes
#include "keyboard_input.h"
#include "error_callback.h"

using namespace std;

// Camera position
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f; // Initial yaw angle

// Timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void frameBufferResizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

string readShaderCode(const char* fileName){
    ifstream meInput(fileName);
    if ( ! meInput.good())
    {
        cout << "File failed to load..." << fileName;
        exit(1);
    }
    return std::string(
        std::istreambuf_iterator<char>(meInput),
        std::istreambuf_iterator<char>());
}

GLFWwindow* initializeWindow() {
    GLFWwindow* window;

    // Set callback for errors
    glfwSetErrorCallback(errorCallback);

    // Initialize the library
    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Without these two hints, nothing above OpenGL version 2.1 is supported
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Running OpenGL on Mac", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    // Set callback for window to close
    glfwSetKeyCallback(window, keyCallback);

    // Set callback for framebuffer on resize
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Used to avoid screen tearing (vsync on)
    glfwSwapInterval(1);

    // Set screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return window;
}

int main(void)
{
    GLFWwindow* window = initializeWindow();
    if (!window)
    {
        // Handle error
        return -1;
    }

    // get window height and width
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Define axis vertices
    float axisLinesVertices[] = {
        // X Axis           // (Red)
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        // Y Axis           // (Green)
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        // Z Axis           // (Blue)
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 10.0f,  0.0f, 0.0f, 1.0f
    };

    // Vertex array object for axis lines
    unsigned int VAOLine;
    glGenVertexArrays(1, &VAOLine);
    glBindVertexArray(VAOLine);

    unsigned int VBOLine;
    glGenBuffers(1, &VBOLine);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisLinesVertices), axisLinesVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Vertex data and buffer
   float vertices[] = {
        // positions          // normals           // colors
        // Back face (red)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,

        // Front face (green)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,

        // Left face (blue)
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

        // Right face (yellow)
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,

        // Bottom face (magenta)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,

        // Top face (cyan)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // setting up first vertex attribute (0)
    glVertexAttribPointer(
        0, // which attribute to configure (0)
        3, // number of floats per vertex (x, y, z)
        GL_FLOAT, // type of data
        GL_FALSE, // should the data be normalized?
        sizeof(float) * 9, // how many floats in each set of data
        0 // where does the data start in the buffer?
    );
    glEnableVertexAttribArray(1); // setting up second vertex attribute (1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(2); // setting up second vertex attribute (1)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));


    // SHADER PROGRAM
    // Vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader
    // read from file
    const char* adapter[1];
    string temp = readShaderCode("shaders/VertexShaderCode.glsl");
    adapter[0] = temp.c_str();
    // set the source
    glShaderSource(vertexShader, 1, adapter, 0);
    glCompileShader(vertexShader);
    // logging
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fputs(infoLog, stderr);
    }
    // Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create shader
    // read from file
    temp = readShaderCode("shaders/FragmentShaderCode.glsl");
    adapter[0] = temp.c_str();
    // set the source
    glShaderSource(fragmentShader, 1, adapter, 0);
    glCompileShader(fragmentShader);
    // logging
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fputs(infoLog, stderr);
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach the shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // logging
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      fputs(infoLog, stderr);
    }
    glUseProgram(shaderProgram);


    // CAMERA TRANSFORMATIONS
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // LIGHTING UNIFORMS
    glm::vec3 lightPos = glm::vec3(10.0f, 0.0f, 0.0f); // Define light position
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White light
    unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    // OpenGL initializations end here

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // setup keyboard input
        processInput(window);
        // Resize the viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        // Clear the color buffer && depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // UPDATE LIGHTING
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

        // UPDATE CAMERA
        // Update cameraFront from cameraYaw
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw));
        front.y = 0; // Keep the camera horizontal
        front.z = sin(glm::radians(cameraYaw));
        cameraFront = glm::normalize(front);
        // Update camera view location
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // DRAW THE CUBE
        // Calculate the cube's rotation
        float timeValue = glfwGetTime();
        float angle = timeValue * glm::radians(50.0f);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Use lighting for rendering cube
        GLint useLightingLocation = glGetUniformLocation(shaderProgram, "useLighting");
        glUniform1i(useLightingLocation, GL_TRUE); 
        // bind the vertex array
        glBindVertexArray(VAO);
        int numCubeVertices = sizeof(vertices) / (sizeof(float) * 6); // Update with your cube's vertices size
        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // DRAW THE AXES LINES
        // Dont use lighting for axes lines
        useLightingLocation = glGetUniformLocation(shaderProgram, "useLighting"); 
        glUniform1i(useLightingLocation, GL_FALSE);
        // bind the vertex array object
        glBindVertexArray(VAOLine); 
        glm::mat4 identityMatrix = glm::mat4(1.0f); // Identity matrix for axes
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identityMatrix));
        // Draw the axes lines
        glDrawArrays(GL_LINES, 0, 6); // 6 vertices for the 3 lines


        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}