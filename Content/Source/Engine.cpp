#include "../Header/Engine.h"
#include <fstream>

#include "../Globals.h"

//PRIVATE 
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
    //create vertices
    Vertex top;
    top.position[0] = 0.0f;
    top.position[1] = 0.5f;
    top.position[2] = 0.0f;

    //top.position = glm::vec3(0.0f, 0.5f, 0.0f);
    //top.color = glm::vec3(1.0f, 0.0f, 0.0f);
    //top.uv = glm::vec2(0.0f, 0.0f);
    Vertex left;
    left.position[0] = -0.5f;
    left.position[1] = -0.5f;
    left.position[2] = 0.0f;

    //left.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    //left.color = glm::vec3(0.0f, 1.0f, 0.0f);
    //left.uv = glm::vec2(1.0f, 1.0f);
    Vertex right;
    right.position[0] = 0.5f;
    right.position[1] = -0.5f;
    right.position[2] = 0.0f;

    //right.position = glm::vec3(0.5f, -0.5f, 0.0f);
    //right.color = glm::vec3(0.0f, 0.0f, 1.0f);
    //right.uv = glm::vec2(1.0f, 1.0f);

    vertices.push_back(left);
    vertices.push_back(right);
    vertices.push_back(top);

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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * (sizeof(GLfloat) * 3), &vertices[0], GL_STATIC_DRAW);

    //create ebo
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //set vertex attribute layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    //create shaders
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, fallback_VShader);
    glAttachShader(shaderProgram, fallback_FShader);

    glLinkProgram(shaderProgram);

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
        //renderer.Render();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

bool Engine::Shutdown()
{
    std::cout << "Shutting down engine..." << std::endl;
    glfwDestroyWindow(window);
    return true;
}
