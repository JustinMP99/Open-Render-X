#include "../Header/Engine.h"

//PRIVATE 
bool Engine::CreateWindow(int width, int height, const char* title)
{

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return false;
    }

    return true;
}

// PUBLIC 

bool Engine::Initialize()
{
    if (!CreateWindow(width, height, "OpenRenderX"))
    {
        std::cout << "Failed to create window!" << std::endl;
        return false;
    }
    return true;
}

void Engine::Loop()
{

    while (glfwWindowShouldClose(window) == false)
    {
        glfwPollEvents();
    }
}

bool Engine::Shutdown()
{
    std::cout << "Shutting down engine..." << std::endl;
    glfwDestroyWindow(window);
    return true;
}
