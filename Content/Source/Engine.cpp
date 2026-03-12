#include "../Header/Engine.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <stb_image.h>
#endif
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

bool Engine::CreateTexture(const char* filepath, unsigned int &texture)
{

    int width;
    int height;
    int nrChannels;

    //Generate texture resource
    glGenTextures(1, &texture);

    //Bind texture so we can perform operations on it
    glBindTexture(GL_TEXTURE_2D, texture);

    //Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //call stbi to flip textures on load
    stbi_set_flip_vertically_on_load(true);

    //load passed in texture image
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Error Loading Texture..." << std::endl;
        return false;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);
    return true;
}

bool Engine::CreateTriangle()
{
    SceneObject *tri = new SceneObject();

    //create vertices
    Vertex top;
    top.position = glm::vec3(0.0f, 0.5f, 0.0f);
    top.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    top.uv = glm::vec2(0.5f, 1.0f);

    Vertex left;
    left.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    left.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    left.uv = glm::vec2(0.0f, 0.0f);

    Vertex right;
    right.position = glm::vec3(0.5f, -0.5f, 0.0f);
    right.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    right.uv = glm::vec2(1.0f, 0.0f);

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

    //create and bind vao
    glGenVertexArrays(1, &tri->mesh->VAO);
    glBindVertexArray(tri->mesh->VAO);

    //create, bind and fill vbo
    glGenBuffers(1, &tri->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tri->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, tri->mesh->vertices.size() * sizeof(Vertex), &tri->mesh->vertices[0], GL_STATIC_DRAW);

    //create, bind and fill ebo
    glGenBuffers(1, &tri->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri->mesh->indices.size() * sizeof(unsigned int), &tri->mesh->indices[0], GL_STATIC_DRAW);

    //set vertex attribute layout
    SetVertexAttributePointers();

    //Create material & set shaders
    tri->material = new Material();
    tri->material->SetShaders(fallback_VShader, fallback_FShader);
    tri->material->SetDiffuseTexture(containerTexture);

    sceneObjects.push_back(tri);

    return true;
}

bool Engine::CreateQuad()
{

    SceneObject *quad = new SceneObject();

    //Create vertices
    Vertex topLeft;
    topLeft.position = glm::vec3(-0.5f, 0.5f, 0.0f);
    topLeft.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    topLeft.uv = glm::vec2(0.0f, 1.0f); 

    Vertex topRight;
    topRight.position = glm::vec3(0.5f, 0.5f, 0.0f);    
    topRight.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    topRight.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeft;
    bottomLeft.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    bottomLeft.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    bottomLeft.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRight;
    bottomRight.position = glm::vec3(0.5f, -0.5f, 0.0f);
    bottomRight.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    bottomRight.uv = glm::vec2(1.0f, 0.0f);

    quad->mesh = new Mesh();

    quad->mesh->vertices.push_back(bottomLeft);
    quad->mesh->vertices.push_back(bottomRight);
    quad->mesh->vertices.push_back(topRight);
    quad->mesh->vertices.push_back(topLeft);

    //Create indices
    quad->mesh->indices.push_back(0);
    quad->mesh->indices.push_back(1);
    quad->mesh->indices.push_back(2);
    quad->mesh->indices.push_back(0);
    quad->mesh->indices.push_back(2);
    quad->mesh->indices.push_back(3);

    quad->mesh->indexCount = static_cast<unsigned int>(quad->mesh->indices.size());

    //Create and bind VAO
    glGenVertexArrays(1, &quad->mesh->VAO);
    glBindVertexArray(quad->mesh->VAO);

    //Create and bind VBO
    glGenBuffers(1, &quad->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, quad->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, quad->mesh->vertices.size() * sizeof(Vertex), &quad->mesh->vertices[0], GL_STATIC_DRAW);

    //Create EBO
    glGenBuffers(1, &quad->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad->mesh->indices.size() * sizeof(unsigned int), &quad->mesh->indices[0], GL_STATIC_DRAW);

    //Set vertex attribute pointers
    SetVertexAttributePointers();

    quad->material = new Material();
    quad->material->SetShaders(fallback_VShader, fallback_FShader);
    quad->material->SetDiffuseTexture(containerTexture);

    quad->position = glm::vec3(0.0f, 0.0f, -3.0f);
    quad->scale = glm::vec3(1.0f, 1.0f, 1.0f);

    sceneObjects.push_back(quad);

    return true;
}

bool Engine::CreateGrid()
{

    //Create SceneObject
    grid = new SceneObject();

    grid->mesh = new Mesh();

    float z = -500.0f;
    float x = -500.0f;

    //Horizonal lines
    for (size_t i = 0; i < gridHorizontal; i++)
    {
        
        //vertices one
        Vertex vertOne;
        vertOne.position = glm::vec3(-100.0f, 0.0f, z);
        vertOne.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        vertOne.uv = glm::vec2(0.0f, 1.0f); 

        //vertices two
        Vertex vertTwo;
        vertTwo.position = glm::vec3(100.0f, 0.0f, z);    
        vertTwo.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        vertTwo.uv = glm::vec2(1.0f, 1.0f);

        grid->mesh->vertices.push_back(vertOne);
        grid->mesh->vertices.push_back(vertTwo);
        z = z + 1.0f;
    }

    //Vertical lines

    for (int i = 0; i < gridVertical; i++)
    {
        //vertices one
        Vertex vertOne;
        vertOne.position = glm::vec3(x, 0.0f, 100.0f);
        vertOne.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        vertOne.uv = glm::vec2(0.0f, 1.0f); 

        //vertices two
        Vertex vertTwo;
        vertTwo.position = glm::vec3(x, 0.0f, -100.0f);    
        vertTwo.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        vertTwo.uv = glm::vec2(1.0f, 1.0f);

        grid->mesh->vertices.push_back(vertOne);
        grid->mesh->vertices.push_back(vertTwo);
        x = x + 1.0f;
    }
    
    grid->mesh->indexCount = static_cast<unsigned int>(grid->mesh->vertices.size());

    glGenVertexArrays(1, &grid->mesh->VAO);
    glBindVertexArray(grid->mesh->VAO);

    //Create and bind VBO
    glGenBuffers(1, &grid->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, grid->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, grid->mesh->vertices.size() * sizeof(Vertex), &grid->mesh->vertices[0], GL_STATIC_DRAW);

    SetVertexAttributePointers();

    grid->material = new Material();
    grid->material->SetShaders(grid_VShader, fallback_FShader);

    grid->position = glm::vec3(0.0f, -3.0f, 0.0f);
    grid->scale = glm::vec3(1.0f, 1.0f, 1.0f);

    //Add grid to scene objects Vector
    //sceneObjects.push_back(grid);

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
    renderer.SetFOV(45.0f);

    if (!renderer.Initialize(window))
    {
        std::cout << "Failed to initialize renderer!" << std::endl;
        return false;
    }

    camera = Camera();
    camera.Setup(glm::vec3(0.0f, 1.0f, 0.0f), window);

    //create shaders
    CreateVertexShader(fallback_VShader, fallbackVertexPath);
    CreateVertexShader(grid_VShader, fallbackVertexPath);
    CreateFragmentShader(fallback_FShader, fallbackFragmentPath);
    CreateFragmentShader(grid_FShader, gridFragmentPath);

    //create textures
    CreateTexture(containerTexturePath, containerTexture);

    //create objects
    //CreateTriangle();
    CreateQuad();
    CreateGrid();

    return true;
}

void Engine::Loop()
{
    while (glfwWindowShouldClose(window) == false)
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ProcessInput();
        camera.Update(deltaTime);
        renderer.UpdateViewMatrix(camera.View);
        //Render Grid
        
        for (unsigned int i = 0; i < sceneObjects.size(); i++)
        {
            renderer.Render(sceneObjects[i]);
        }
        renderer.RenderGrid(grid);
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
