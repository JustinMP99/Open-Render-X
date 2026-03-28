#pragma once
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
    float camSpeed = 2.5f;
    float camFastSpeed = 5.0f;
    bool firstMouse = true;
    bool captureMouse = false;
    bool moveFast = false;
    double currentX = 0.0f;
    double currentY = 0.0f;
    double lastX = 0;
    double lastY = 0;
    float yaw = -90.0f;
    float pitch = 0.0f;
    int windowWidth = 0;
    int windowHeight = 0;

    void ProcessInput(float deltaTime);

    void CaptureMouse();
    
public:
    glm::mat4 View;
    GLFWwindow* window;

    bool Setup(glm::vec3 startPosition, GLFWwindow* newWindow);
    
    void Update(float deltaTime);

};