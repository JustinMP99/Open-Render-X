#include "../Header/Engine.h"

//PRIVATE 
bool Engine::CreateTriangle()
{

    //create vertices
    Vertex top;
    top.position = glm::vec3(0.0f, 0.5f, 0.0f);
    top.color = glm::vec3(1.0f, 0.0f, 0.0f);
    top.uv = glm::vec2(0.0f, 0.0f);
    Vertex left;
    left.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    left.color = glm::vec3(0.0f, 1.0f, 0.0f);
    left.uv = glm::vec2(1.0f, 1.0f);
    Vertex right;
    right.position = glm::vec3(0.5f, -0.5f, 0.0f);
    right.color = glm::vec3(0.0f, 0.0f, 1.0f);
    right.uv = glm::vec2(1.0f, 1.0f);

    vertices.push_back(top);
    vertices.push_back(left);
    vertices.push_back(right);

    //create indices
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    //create vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create vbo
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //create ebo
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //create shaders

    


    return true;
}

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

    if (!renderer.Initialize(window))
    {
        std::cout << "Failed to initialize renderer!" << std::endl;
        return false;
    }

    return true;
}

void Engine::Loop()
{

    while (glfwWindowShouldClose(window) == false)
    {


        renderer.Render();

        glfwPollEvents();
    }
}

bool Engine::Shutdown()
{
    std::cout << "Shutting down engine..." << std::endl;
    glfwDestroyWindow(window);
    return true;
}
