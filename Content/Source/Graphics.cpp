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

void Graphics::Render(SceneObject* object)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(object->material->shaderProgram);
    glBindVertexArray(object->mesh->VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

bool Graphics::Shutdown()
{
    return true;
}