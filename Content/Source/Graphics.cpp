#include "../Header/Graphics.h"



// PRIVATE 



// PUBLIC

bool Graphics::Initialize(GLFWwindow* win)
{
    window = win;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
    return true;
}

void Graphics::Render()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
}

bool Graphics::Shutdown()
{
    return true;
}