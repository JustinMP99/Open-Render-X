//
// Created by Justin Philie on 6/24/26.
//

#include "../Header/DisplayHandler.h"

//PRIVATE

bool DisplayHandler::CreateGLFWWindow()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, appleTitle, nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    //windows.push_back(window);

    return true;
}

bool DisplayHandler::CreateNativeWindow()
{

#ifdef Platform_Apple
    
#endif

}

//PUBLIC

DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{
}

bool DisplayHandler::Initialize()
{


    return true;
}

bool DisplayHandler::CreateWindow()
{
    if (displayMode == DisplayMode::GLFW)
    {
        CreateGLFWWindow();
    }
    else if (displayMode == DisplayMode::NATIVE)
    {

    }
}

bool DisplayHandler::Shutdown()
{
    for (int i = 0; i < windows.size(); i++)
    {
        glfwDestroyWindow(windows[i]);
    }

    glfwDestroyWindow(window);

}

bool DisplayHandler::SwapBuffers()
{
    for (int i = 0; i < windows.size(); i++)
    {
        glfwSwapBuffers(windows[i]);
    }
    glfwSwapBuffers(window);
    return true;
}

int DisplayHandler::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow *DisplayHandler::GetWindow()
{
    return window;
}

int DisplayHandler::GetWindowWidth()
{
    return window_width;
}

int DisplayHandler::GetWindowHeight()
{
    return window_height;
}

void DisplayHandler::SetWindowSize(unsigned int width, unsigned int height)
{
    window_width = width;
    window_height = height;
}

void DisplayHandler::SetDisplayMode(DisplayMode mode)
{
    displayMode = mode;
}
