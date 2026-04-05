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

    object->transform = glm::translate(object->transform, object->position);
    object->transform = glm::scale(object->transform, object->scale);
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    
    //object->transform = glm::rotate(object->transform, glm::radians(float(glfwGetTime()) * -10.0f), glm::vec3(0.0, 1.0, 0.0));
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    
    object->transform = glm::rotate(object->transform, glm::radians(object->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Graphics::UpdateTransformUniforms(SceneObject* object)
{
    //update objects transform uniform in shader
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    glBufferSubData(GL_UNIFORM_BUFFER, 2 *sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(object->transform));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Graphics::UpdateViewPosition(glm::vec3 viewPos)
{
    int viewPosLocation = glGetUniformLocation(uboMatrices, "viewPos");
    glUniform3f(viewPosLocation, viewPos.x, viewPos.y, viewPos.z);
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

    //Create Uniform Buffer Object for view and projection matrices
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


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

    if (object->mesh->useEBO)
    {
        glDrawElements(drawMode, object->mesh->indexCount, GL_UNSIGNED_INT, 0);
        //glBufferData(GL_ARRAY_BUFFER, object->mesh->vertices.size() * sizeof(Vertex), &object->mesh->vertices[0], GL_STATIC_DRAW);
        //glDrawArrays(GL_TRIANGLES, 0, object->mesh->indexCount);
    }
    else if (!object->mesh->useEBO)
    {
        glDrawArrays(drawMode, 0, object->mesh->indexCount);
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