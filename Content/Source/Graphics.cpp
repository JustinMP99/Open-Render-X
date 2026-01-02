#include "../Header/Graphics.h"



// PRIVATE 



// PUBLIC
Graphics::Graphics()
{

}

Graphics::Graphics(int width, int height)
{
    width = width;
    height = height;
}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(GLFWwindow* win)
{
    window = win;
    glfwGetWindowSize(window, &width,  &height);
    glViewport(0, 0, width,  height);
	glEnable(GL_DEPTH_TEST);
    return true;
}

void Graphics::Render()
{

}

bool Graphics::Shutdown()
{
    return true;
}