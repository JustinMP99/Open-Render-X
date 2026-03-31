#include "../Header/Graphics.h"



// PRIVATE 

void Graphics::ClearScreen()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::ClearScreen(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::SetModel(SceneObject* object)
{
    //reset the objects transform to identity before applying position, rotation, and scale
    object->transform = glm::mat4(1.0f);

    object->transform = glm::scale(object->transform, object->scale);
    object->transform = glm::translate(object->transform, object->position);
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Graphics::UpdateTransformUniforms(SceneObject* object)
{
    //update objects transform uniform in shader
	unsigned int modelLoc = glGetUniformLocation(object->material->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(object->transform));
	
	int viewLoc = glGetUniformLocation(object->material->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projectionLoc = glGetUniformLocation(object->material->shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

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
    projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
    SetViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
    return true;
}

void Graphics::Render()
{

}

void Graphics::Render(SceneObject* object)
{
   
    object->material->Use();

    //set model matrix for object
    SetModel(object);
    
    //update shader uniforms
    UpdateTransformUniforms(object);
    
    glBindVertexArray(object->mesh->VAO);

    int drawMode = object->mesh->GetDrawMode();
    if (drawMode == GL_TRIANGLES)
    {
        glDrawElements(GL_TRIANGLES, object->mesh->indexCount, GL_UNSIGNED_INT, 0);
    }
    else if (drawMode == GL_LINES)
    {
        glDrawArrays(GL_LINES, 0, object->mesh->indexCount);
    }
}

bool Graphics::Shutdown()
{
    return true;
}

void Graphics::UpdateViewMatrix(glm::mat4 viewMatrix)
{
    view = viewMatrix;
}

void Graphics::SetFOV(float newFOV)
{
    fov = newFOV;
}

void Graphics::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}