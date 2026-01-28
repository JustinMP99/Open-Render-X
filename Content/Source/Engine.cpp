#include "../Header/Engine.h"

//PRIVATE
void Engine::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

std::string Engine::LoadShaderAsString(const std::string& filename)
{
    std::string result = "";
    std::string line = "";
    //std::ifstream shaderFile(filename.c_str());
    std::ifstream shaderFile;
    shaderFile.open(filename);
    if (!shaderFile.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filename);
    }
    if (shaderFile.is_open())
    {
        while (std::getline(shaderFile, line))
        {
            result += line + "\n";
        }
        shaderFile.close();
    }
    return result;
}

bool Engine::CreateVertexShader(unsigned int &shader, const char *shaderPath)
{
    std::string vertexShader = LoadShaderAsString(shaderPath);
    const char* vShader = vertexShader.c_str();
    int success;
    char infoLog[512];

    shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &vShader, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Failed to compile vertex shader: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Engine::CreateFragmentShader(unsigned int &shader, const char *shaderPath)
{
    std::string fragShader = LoadShaderAsString(shaderPath);
    const char* fShader = fragShader.c_str();
    int success;
    char infoLog[512];
    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &fShader, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Failed to compile fragment shader: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Engine::CreateTriangle()
{
    SceneObject *tri = new SceneObject();

    std::vector<Vertex> tempVerts;
    std::vector<unsigned int> tempIndices;
    //create vertices
    Vertex top;
    top.position = glm::vec3(0.0f, 0.5f, 0.0f);
    top.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    top.uv = glm::vec2(0.0f, 0.0f);

    Vertex left;
    left.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    left.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    left.uv = glm::vec2(1.0f, 1.0f);

    Vertex right;
    right.position = glm::vec3(0.5f, -0.5f, 0.0f);
    right.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    right.uv = glm::vec2(1.0f, 1.0f);

    tri->mesh = new Mesh();

    //add vertices to vector
    tri->mesh->vertices.push_back(left);
    tri->mesh->vertices.push_back(right);
    tri->mesh->vertices.push_back(top);

    //create indices & add vertices to vector
    tri->mesh->indices.push_back(0);
    tri->mesh->indices.push_back(1);
    tri->mesh->indices.push_back(2);

    tri->mesh->indexCount = static_cast<unsigned int>(tri->mesh->indices.size());

    //create vao
    glGenVertexArrays(1, &tri->mesh->VAO);
    glBindVertexArray(tri->mesh->VAO);

    //create vbo
    glGenBuffers(1, &tri->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tri->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, tri->mesh->vertices.size() * sizeof(Vertex), &tri->mesh->vertices[0], GL_STATIC_DRAW);

    //create ebo
    glGenBuffers(1, &tri->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri->mesh->indices.size() * sizeof(unsigned int), &tri->mesh->indices[0], GL_STATIC_DRAW);

    //set vertex attribute layout
    SetVertexAttributePointers();

    //Create material & set shaders
    tri->material = new Material();
    tri->material->SetShaders(fallback_VShader, fallback_FShader);

    sceneObjects.push_back(tri);

    return true;
}

bool Engine::CreateWindow(int width, int height, const char* title)
{

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

void Engine::SetVertexAttributePointers()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
}

// PUBLIC

bool Engine::Initialize()
{
    if (!CreateWindow(width,  height,  appleTitle))
    {
        std::cout << "Failed to create window!" << std::endl;
        return false;
    }

    renderer = Graphics(width, height);

    if (!renderer.Initialize(window))
    {
        std::cout << "Failed to initialize renderer!" << std::endl;
        return false;
    }

    //create shaders
    CreateVertexShader(fallback_VShader, fallbackVertexPath);
    CreateFragmentShader(fallback_FShader, fallbackFragmentPath);

    //create objects
    CreateTriangle();

    return true;
}

void Engine::Loop()
{
    while (glfwWindowShouldClose(window) == false)
    {
        ProcessInput();
        for (unsigned int i = 0; i < sceneObjects.size(); i++)
        {
            renderer.Render(sceneObjects[i]);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

bool Engine::Shutdown()
{
    std::cout << "Shutting down engine..." << std::endl;
    for (unsigned int i = 0; i < sceneObjects.size(); i++)
    {
        delete sceneObjects[i];
    }
    glfwDestroyWindow(window);
    return true;
}
