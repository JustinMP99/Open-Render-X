#include "../Header/Camera.h"

// PRIVATE

void Camera::ProcessInput(float deltaTime)
{
    float finalSpeed = camSpeed * deltaTime;

    //QUIT APPLICATION
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    //MOUSE
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        captureMouse = true;
    }
    else if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        captureMouse = false;
        firstMouse = true;
    }
    
    //WASD MOVEMENT
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= finalSpeed * cameraDir;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += finalSpeed * cameraDir;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraPos -= glm::normalize(glm::cross(cameraDir, up)) * finalSpeed ;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraPos += glm::normalize(glm::cross(cameraDir, up)) * finalSpeed;
    }

    //UP & DOWN
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPos += finalSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraPos -= finalSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void Camera::CaptureMouse()
{

    glfwGetCursorPos(window, &currentX, &currentY);

    if (firstMouse)
    {
        lastX = currentX;
        lastY = currentY;
        firstMouse = false;
    }

    //calculate offset values
    float xoffset = currentX - lastX;
    float yoffset = lastY - currentY;
    lastX = currentX;
    lastY = currentY;

    //apply sensitivity
    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDir = glm::normalize(direction);
}

// PUBLIC
bool Camera::Setup(glm::vec3 startPosition, GLFWwindow* newWindow)
{
    //set window
    window = newWindow;
    if (window == nullptr)
    {
        std::cout << "Window is null" << std::endl;
    }

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    lastX = windowWidth / 2;
    lastY = windowHeight / 2;
    
    //set position
    cameraPos = startPosition;
    
    View = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
    return true;
}

void Camera::Update(float deltaTime)
{

    //process input
    ProcessInput(deltaTime);

    if (captureMouse)
    {
        CaptureMouse();
    }
    
    //update view matrix
    View = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
}
