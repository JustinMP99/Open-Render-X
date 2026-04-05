#include "../Header/Engine.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <stb_image.h>
#endif

//PRIVATE
void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    renderer.SetViewport(0, 0, width, height);
}

float Engine::GetRandomFloat(float min, float max)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // 2. Seed a pseudo-random number generator (e.g., Mersenne Twister)
    std::mt19937 gen(seed); // A high-quality engine

    // 3. Define the desired range (inclusive) using a distribution
    std::uniform_real_distribution<float> distrib(min, max);

    // 4. Generate the random number
    return distrib(gen);
}

#pragma region Program Loop Functions

void Engine::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        renderDebugWindow = true;
    }
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        renderDebugWindow = false;
    }

    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        renderSceneList = true;
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        renderSceneList = false;
    }


}

void Engine::CalculateDelta()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Engine::CreateDebugSettingsWindow()
{

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::Begin("Debug Menu", nullptr, ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_AlwaysAutoResize);
    //ImGui::BeginGroup();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Data");
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
    ImGui::Text("Unlit Scene Object Count: %d", sceneObjects.size());
    ImGui::Text("Lit Scene Object Count: %d", litSceneObjects.size());
    ImGui::Separator();

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Settings");

    ImGui::Checkbox("Render Grid", &renderGrid);
    if (ImGui::Checkbox("Enable Wireframe", &enableWireframe))
    {
        if (enableWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }  
    };
    
    ImGui::Spacing();

    ImGui::ColorEdit3("Clear Color", (float*)&clearScreenColor);

    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Lighting");
    ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
    ImGui::ColorEdit3("Ambient Color", (float*)&ambientColor);

    ImGui::SliderFloat("Light Strength", &lightStrength, 0.0f, 1.0f);
    ImGui::ColorEdit3("Light Color", (float*)&lightColor);
    ImGui::InputFloat3("Light Position", (float*)&lightPos);

    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scene Object Creation");

    if (ImGui::Button("Create Quad"))
    {
        CreateQuad();
    }

    ImGui::SameLine();

    if (ImGui::Button("Create Cube"))
    {
        CreateCube();
    }

    ImGui::SameLine();

    if (ImGui::Button("Create Sphere"))
    {
        CreateSphere();
    }
    
    if(ImGui::Button("Clear SceneObjects"))
    {
        ClearSceneObjects();
    }

    ImGui::End();
}

void Engine::CreateSceneListWindow()
{

    ImGui::SetNextWindowPos(ImVec2(width / 2, height / 2), ImGuiCond_Once);
    ImGui::Begin("Scene List", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    int count = 0;

    for (int i = 0; i < litSceneObjects.size(); i++)
    {

        if (ImGui::Button((litSceneObjects[i]->name).c_str()))
        {
            selectedSceneObject = litSceneObjects[count];
            renderInspectorWindow = true;
            std::cout << "> Selected SceneObject: " << selectedSceneObject->name.c_str() << std::endl;
        }

        count++;
    }

    count = 0;

    for (int i = 0; i < sceneObjects.size(); i++)
    {
        if (ImGui::Button((sceneObjects[i]->name).c_str()))
        {
            selectedSceneObject = sceneObjects[count];
            renderInspectorWindow = true;
            std::cout << "> Selected SceneObject: " << selectedSceneObject->name.c_str() << std::endl;
        }

        count++;
    }


    ImGui::End();
}

void Engine::CreateInspectorWindow()
{

    if (selectedSceneObject != NULL)
    {

        ImGui::Begin("Inspector");

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Core Data");

        ImGui::Text("Name: %s", selectedSceneObject->name.c_str());
        ImGui::InputFloat3("Position", (float*)&selectedSceneObject->position);
        ImGui::InputFloat3("Rotation", (float*)&selectedSceneObject->rotation);
        ImGui::InputFloat3("Scale", (float*)&selectedSceneObject->scale);

        ImGui::Separator();

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Mesh Data");
        ImGui::Text("Vertex Count: %d", selectedSceneObject->mesh->vertices.size());
        ImGui::Text("Index Count: %d", selectedSceneObject->mesh->indices.size());

        ImGui::End();

    }

}

#pragma endregion

#pragma region Setup Utility Functions

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

void Engine::ReadOBJ(std::string filepath, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string &objectName)
{

    FILE* file = fopen(filepath.c_str(), "r");
    if (file == nullptr)
    {
        std::cout << "> Could not open file: " << filepath << std::endl;
    }

    std::vector<glm::vec3> temp_position;
    std::vector<glm::vec3> temp_normal;
    std::vector<glm::vec2> temp_uv;

    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> positionIndices;
    std::vector<unsigned int> normalIndices;

    while(1)
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 position;
            fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
            temp_position.push_back(position);
            std::cout << "> Read vertex: " << "X " << position.x << " Y " << position.y << " Z " << position.z << std::endl;
        }
        else if( strcmp(lineHeader, "vt") == 0 )
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uv.push_back(uv);
        }
        else if( strcmp(lineHeader, "vn") == 0 )
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normal.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0], &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser -> Try exporting with other options\n");
                break;
            }

            //out_indices.push_back(positionIndex[0] - 1);
            //out_indices.push_back(positionIndex[1] - 1);
            //out_indices.push_back(positionIndex[2] - 1);
            positionIndices.push_back(positionIndex[0]);
            positionIndices.push_back(positionIndex[1]);
            positionIndices.push_back(positionIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);


        }
        else if(strcmp(lineHeader, "o") == 0)
        {
            char name[128];
            fscanf(file, "%s\n", name);
            objectName = name;

        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

        if (res == EOF)
        {
            break;
        }
    }

    for (int i = 0; i < positionIndices.size(); i++)
    {
        Vertex vert;
        unsigned int positionIndex = positionIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];
        vert.position = temp_position[positionIndex - 1];
        vert.uv = temp_uv[uvIndex - 1];
        vert.normal = temp_normal[normalIndex - 1];
        out_indices.push_back(i);
        out_vertices.push_back(vert);
    }

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
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        engine->framebuffer_size_callback(win, width, height);
    });

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

void Engine::ImGuiSetup()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

#pragma endregion

#pragma region SceneObject Creation

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

        if (z != 0.0f)
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
        }

        z = z + 1.0f;
    }

    //Vertical lines

    for (int i = 0; i < gridVertical; i++)
    {
        if (x != 0.0f)
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
        }

        x = x + 1.0f;
    }

    grid->mesh->indexCount = static_cast<int>(grid->mesh->vertices.size());

    glGenVertexArrays(1, &grid->mesh->VAO);
    glBindVertexArray(grid->mesh->VAO);

    //Create and bind VBO
    glGenBuffers(1, &grid->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, grid->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, grid->mesh->vertices.size() * sizeof(Vertex), &grid->mesh->vertices[0], GL_STATIC_DRAW);
    SetVertexAttributePointers();
    grid->mesh->SetDrawMode(DrawMode::LINES);
    grid->material = new Material();
    grid->material->SetShaders(grid_VShader, grid_FShader);
    grid->position = glm::vec3(0.0f, 0.0f, 0.0f);
    grid->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    grid->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    int colorLocation = glGetUniformLocation(grid->material->shaderProgram, "color");
    glUseProgram(grid->material->shaderProgram);
    glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);

    //sceneObjects.push_back(grid);

    return true;
}

bool Engine::CreateRays()
{

    xRay = new SceneObject();
    yRay = new SceneObject();
    zRay = new SceneObject();

    xRay->mesh = new Mesh();
    yRay->mesh = new Mesh();
    zRay->mesh = new Mesh();

    Vertex xVertOne;
    xVertOne.position = glm::vec3(-500.0f, 0.0f, 0.0f);
    xVertOne.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    xVertOne.uv = glm::vec2(0.0f, 0.0f);
    Vertex xVertTwo;
    xVertTwo.position = glm::vec3(500.0f, 0.0f, 0.0f);
    xVertTwo.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    xVertTwo.uv = glm::vec2(1.0f, 1.0f);


    xRay->mesh->vertices.push_back(xVertOne);
    xRay->mesh->vertices.push_back(xVertTwo);

    xRay->mesh->SetDrawMode(DrawMode::LINES);
    xRay->material = new Material();

    xRay->mesh->indices.push_back(0);
    xRay->mesh->indices.push_back(1);

    xRay->mesh->indexCount = static_cast<unsigned int>(xRay->mesh->indices.size());

    glGenVertexArrays(1, &xRay->mesh->VAO);
    glBindVertexArray(xRay->mesh->VAO);

    glGenBuffers(1, &xRay->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, xRay->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, xRay->mesh->vertices.size() * sizeof(Vertex), &xRay->mesh->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &xRay->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, xRay->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, xRay->mesh->indices.size() * sizeof(unsigned int), &xRay->mesh->indices[0], GL_STATIC_DRAW);

    SetVertexAttributePointers();

    xRay->material->SetShaders(grid_VShader, grid_FShader);
    xRay->position = glm::vec3(0.0f, 0.0f, 0.0f);
    xRay->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    xRay->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    int colorLocation = glGetUniformLocation(xRay->material->shaderProgram, "color");
    glUseProgram(xRay->material->shaderProgram);
    glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);


    Vertex yVertOne;
    yVertOne.position = glm::vec3(0.0f, -500.0f, 0.0f);
    yVertOne.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    yVertOne.uv = glm::vec2(0.0f, 0.0f);
    Vertex yVertTwo;
    yVertTwo.position = glm::vec3(0.0f, 500.0f, 0.0f);
    yVertTwo.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    yVertTwo.uv = glm::vec2(1.0f, 1.0f);


    yRay->mesh->vertices.push_back(yVertOne);
    yRay->mesh->vertices.push_back(yVertTwo);

    yRay->mesh->SetDrawMode(DrawMode::LINES);
    yRay->material = new Material();

    yRay->mesh->indices.push_back(0);
    yRay->mesh->indices.push_back(1);

    yRay->mesh->indexCount = static_cast<unsigned int>(yRay->mesh->indices.size());

    glGenVertexArrays(1, &yRay->mesh->VAO);
    glBindVertexArray(yRay->mesh->VAO);

    glGenBuffers(1, &yRay->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, yRay->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, yRay->mesh->vertices.size() * sizeof(Vertex), &yRay->mesh->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &yRay->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, yRay->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, yRay->mesh->indices.size() * sizeof(unsigned int), &yRay->mesh->indices[0], GL_STATIC_DRAW);

    SetVertexAttributePointers();

    yRay->material->SetShaders(grid_VShader, grid_FShader);
    yRay->position = glm::vec3(0.0f, 0.0f, 0.0f);
    yRay->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    yRay->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    colorLocation = glGetUniformLocation(yRay->material->shaderProgram, "color");
    glUseProgram(yRay->material->shaderProgram);
    glUniform3f(colorLocation, 0.0f, 1.0f, 0.0f);


    Vertex zVertOne;
    zVertOne.position = glm::vec3(0.0f, 0.0f, -500.0f);
    zVertOne.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    zVertOne.uv = glm::vec2(0.0f, 0.0f);
    Vertex zVertTwo;
    zVertTwo.position = glm::vec3(0.0f, 0.0f, 500.0f);
    zVertTwo.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    zVertTwo.uv = glm::vec2(1.0f, 1.0f);


    zRay->mesh->vertices.push_back(zVertOne);
    zRay->mesh->vertices.push_back(zVertTwo);

    zRay->mesh->SetDrawMode(DrawMode::LINES);
    zRay->material = new Material();

    zRay->mesh->indices.push_back(0);
    zRay->mesh->indices.push_back(1);

    zRay->mesh->indexCount = static_cast<unsigned int>(zRay->mesh->indices.size());

    glGenVertexArrays(1, &zRay->mesh->VAO);
    glBindVertexArray(zRay->mesh->VAO);

    glGenBuffers(1, &zRay->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, zRay->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, zRay->mesh->vertices.size() * sizeof(Vertex), &zRay->mesh->vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &zRay->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, zRay->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, zRay->mesh->indices.size() * sizeof(unsigned int), &zRay->mesh->indices[0], GL_STATIC_DRAW);

    SetVertexAttributePointers();

    zRay->material->SetShaders(grid_VShader, grid_FShader);
    zRay->position = glm::vec3(0.0f, 0.0f, 0.0f);
    zRay->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    zRay->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    colorLocation = glGetUniformLocation(zRay->material->shaderProgram, "color");
    glUseProgram(zRay->material->shaderProgram);
    glUniform3f(colorLocation, 0.0f, 0.0f, 1.0f);

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
    //tri->material->SetDiffuseTexture(containerTexture);

    float min = -5.0f;
    float max = 5.0f;

    // 4. Generate the random number
    float random_x = GetRandomFloat(min, max);
    float random_z = GetRandomFloat(min, max);
    tri->position = glm::vec3(random_x, 0.0f, -random_z);
    tri->scale = glm::vec3(1.0f, 1.0f, 1.0f);

    sceneObjects.push_back(tri);

    return true;
}

bool Engine::CreateQuad()
{

    SceneObject *quad = new SceneObject();

    quad->name = "Quad " + std::to_string(litSceneObjects.size());

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

    //ReadOBJ((linuxProjectDirectory + objModelPath).c_str(), quad->mesh->vertices, quad->mesh->indices);

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

    quad->mesh->SetDrawMode(DrawMode::TRIANGLES);

    quad->mesh->useEBO = true;

    quad->material = new Material();
    quad->material->SetShaders(fallback_VShader, simpleLit_FShader);
    quad->material->SetDiffuseTexture(containerTexture);

    float min = -5.0f;
    float max = 5.0f;

    // 4. Generate the random number
    float random_x = GetRandomFloat(min, max);
    float random_y = GetRandomFloat(min, max);
    float random_z = GetRandomFloat(min, max);
    quad->position = glm::vec3(random_x, 0.0f, -random_z);
    quad->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    quad->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    litSceneObjects.push_back(quad);

    return true;

}

bool Engine::CreateCube_Old()
{

    //Create SceneObject
    SceneObject *cube = new SceneObject();

    cube->name = "Cube " + std::to_string(sceneObjects.size());

    //Create Vertices

    //Front Vertices
    Vertex topLeftFront;
    topLeftFront.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    topLeftFront.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    topLeftFront.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightFront;
    topRightFront.position = glm::vec3(0.5f, 0.5f, 0.5f);
    topRightFront.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    topRightFront.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftFront;
    bottomLeftFront.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    bottomLeftFront.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    bottomLeftFront.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightFront;
    bottomRightFront.position = glm::vec3(0.5f, -0.5f, 0.5f);
    bottomRightFront.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    bottomRightFront.uv = glm::vec2(1.0f, 0.0f);

    //Back Vertices
    Vertex topLeftBack;
    topLeftBack.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    topLeftBack.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    topLeftBack.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightBack;
    topRightBack.position = glm::vec3(0.5f, 0.5f, -0.5f );
    topRightBack.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    topRightBack.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftBack;
    bottomLeftBack.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    bottomLeftBack.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    bottomLeftBack.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightBack;
    bottomRightBack.position = glm::vec3(0.5f, -0.5f, -0.5f);
    bottomRightBack.normal = glm::vec3(0.0f, 0.0f, -1.0f);
    bottomRightBack.uv = glm::vec2(1.0f, 0.0f);

    //Left Vertices
    Vertex topLeftLeft;
    topLeftLeft.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    topLeftLeft.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    topLeftLeft.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightLeft;
    topRightLeft.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    topRightLeft.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    topRightLeft.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftLeft;
    bottomLeftLeft.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    bottomLeftLeft.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    bottomLeftLeft.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightLeft;
    bottomRightLeft.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    bottomRightLeft.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    bottomRightLeft.uv = glm::vec2(1.0f, 0.0f);

    //Right Vertices
    Vertex topLeftRight;
    topLeftRight.position = glm::vec3(0.5f, 0.5f, -0.5f);
    topLeftRight.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    topLeftRight.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightRight;
    topRightRight.position = glm::vec3(0.5f, 0.5f, 0.5f);
    topRightRight.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    topRightRight.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftRight;
    bottomLeftRight.position = glm::vec3(0.5f, -0.5f, -0.5f);
    bottomLeftRight.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    bottomLeftRight.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightRight;
    bottomRightRight.position = glm::vec3(0.5f, -0.5f, 0.5f);
    bottomRightRight.normal = glm::vec3(1.0f, 0.0f, 0.0f);
    bottomRightRight.uv = glm::vec2(1.0f, 0.0f);

    //Top Vertices
    Vertex topLeftTop;
    topLeftTop.position = glm::vec3(-0.5f, 0.5f, -0.5f);
    topLeftTop.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    topLeftTop.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightTop;
    topRightTop.position = glm::vec3(0.5f, 0.5f, -0.5f);
    topRightTop.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    topRightTop.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftTop;
    bottomLeftTop.position = glm::vec3(-0.5f, 0.5f, 0.5f);
    bottomLeftTop.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    bottomLeftTop.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightTop;
    bottomRightTop.position = glm::vec3(0.5f, 0.5f, 0.5f);
    bottomRightTop.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    bottomRightTop.uv = glm::vec2(1.0f, 0.0f);

    //Bottom Vertices
    Vertex topLeftBottom;
    topLeftBottom.position = glm::vec3(-0.5f, -0.5f, -0.5f);
    topLeftBottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    topLeftBottom.uv = glm::vec2(0.0f, 1.0f);

    Vertex topRightBottom;
    topRightBottom.position = glm::vec3(0.5f, -0.5f, -0.5f);
    topRightBottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    topRightBottom.uv = glm::vec2(1.0f, 1.0f);

    Vertex bottomLeftBottom;
    bottomLeftBottom.position = glm::vec3(-0.5f, -0.5f, 0.5f);
    bottomLeftBottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    bottomLeftBottom.uv = glm::vec2(0.0f, 0.0f);

    Vertex bottomRightBottom;
    bottomRightBottom.position = glm::vec3(0.5f, -0.5f, 0.5f);
    bottomRightBottom.normal = glm::vec3(0.0f, -1.0f, 0.0f);
    bottomRightBottom.uv = glm::vec2(1.0f, 0.0f);

    cube->mesh = new Mesh();

    //Front
    cube->mesh->vertices.push_back(bottomLeftFront); //0
    cube->mesh->vertices.push_back(bottomRightFront); //1
    cube->mesh->vertices.push_back(topRightFront); //2
    cube->mesh->vertices.push_back(topLeftFront); //3

    //Back
    cube->mesh->vertices.push_back(bottomLeftBack); //4
    cube->mesh->vertices.push_back(bottomRightBack); //5
    cube->mesh->vertices.push_back(topRightBack); //6
    cube->mesh->vertices.push_back(topLeftBack); //7

    //Left
    cube->mesh->vertices.push_back(bottomLeftLeft); //8
    cube->mesh->vertices.push_back(bottomRightLeft); //9
    cube->mesh->vertices.push_back(topRightLeft); //10
    cube->mesh->vertices.push_back(topLeftLeft); //11

    //Right
    cube->mesh->vertices.push_back(bottomLeftRight); //12
    cube->mesh->vertices.push_back(bottomRightRight); //13
    cube->mesh->vertices.push_back(topRightRight); //14
    cube->mesh->vertices.push_back(topLeftRight); //15

    //Top
    cube->mesh->vertices.push_back(bottomLeftTop); //16
    cube->mesh->vertices.push_back(bottomRightTop); //17
    cube->mesh->vertices.push_back(topRightTop); //18
    cube->mesh->vertices.push_back(topLeftTop); //19

    //Bottom
    cube->mesh->vertices.push_back(bottomLeftBottom); //20
    cube->mesh->vertices.push_back(bottomRightBottom); //21
    cube->mesh->vertices.push_back(topRightBottom); //22
    cube->mesh->vertices.push_back(topLeftBottom); //23

    //Create Indices

    //Front
    cube->mesh->indices.push_back(0);
    cube->mesh->indices.push_back(1);
    cube->mesh->indices.push_back(2);

    cube->mesh->indices.push_back(0);
    cube->mesh->indices.push_back(2);
    cube->mesh->indices.push_back(3);

    //Back
    cube->mesh->indices.push_back(4);
    cube->mesh->indices.push_back(5);
    cube->mesh->indices.push_back(6);

    cube->mesh->indices.push_back(4);
    cube->mesh->indices.push_back(6);
    cube->mesh->indices.push_back(7);

    //Left
    cube->mesh->indices.push_back(8);
    cube->mesh->indices.push_back(9);
    cube->mesh->indices.push_back(10);

    cube->mesh->indices.push_back(8);
    cube->mesh->indices.push_back(10);
    cube->mesh->indices.push_back(11);

    //Right
    cube->mesh->indices.push_back(12);
    cube->mesh->indices.push_back(13);
    cube->mesh->indices.push_back(14);

    cube->mesh->indices.push_back(12);
    cube->mesh->indices.push_back(14);
    cube->mesh->indices.push_back(15);

    //Top
    cube->mesh->indices.push_back(16);
    cube->mesh->indices.push_back(17);
    cube->mesh->indices.push_back(18);

    cube->mesh->indices.push_back(16);
    cube->mesh->indices.push_back(18);
    cube->mesh->indices.push_back(19);

    //Bottom
    cube->mesh->indices.push_back(20);
    cube->mesh->indices.push_back(21);
    cube->mesh->indices.push_back(22);

    cube->mesh->indices.push_back(20);
    cube->mesh->indices.push_back(22);
    cube->mesh->indices.push_back(23);

    cube->mesh->indexCount = static_cast<unsigned int>(cube->mesh->indices.size());

    std::cout << "Cube Vertex Count: " << cube->mesh->vertices.size() << std::endl;

    cube->mesh->SetDrawMode(DrawMode::TRIANGLES);

    cube->mesh->useEBO = true;

    //create vao
    glGenVertexArrays(1, &cube->mesh->VAO);
    glBindVertexArray(cube->mesh->VAO);

    //create vbo
    glGenBuffers(1, &cube->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, cube->mesh->vertices.size() * sizeof(Vertex), &cube->mesh->vertices[0], GL_STATIC_DRAW);

    //create ebo
    glGenBuffers(1, &cube->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->mesh->indices.size() * sizeof(unsigned int), &cube->mesh->indices[0], GL_STATIC_DRAW);

    SetVertexAttributePointers();

    //create material
    cube->material = new Material();

    //set shaders
    cube->material->SetShaders(fallback_VShader, simpleLit_FShader);
    cube->material->SetDiffuseTexture(containerTexture);

    // 3. Define the desired range (inclusive) using a distribution
    float min = -5.0f;
    float max = 5.0f;

    float random_x = GetRandomFloat(min, max);
    float random_z = GetRandomFloat(min, max);

    //set initial position and scale
    cube->position = glm::vec3(random_x, 0.0f, -random_z);
    cube->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    cube->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    //add to scene objects vector
    litSceneObjects.push_back(cube);

    return true;
}

bool Engine::CreateSphere()
{
    SceneObject *obj = new SceneObject();
    obj->mesh = new Mesh();
    obj->material = new Material();

    ReadOBJ((projectDirectory + sphereMeshPath).c_str(), obj->mesh->vertices, obj->mesh->indices, obj->name);

    obj->mesh->useEBO = true;

    obj->mesh->indexCount = static_cast<unsigned int>(obj->mesh->indices.size());

    glGenVertexArrays(1, &obj->mesh->VAO);
    glBindVertexArray(obj->mesh->VAO);

    glGenBuffers(1, &obj->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, obj->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->mesh->vertices.size() * sizeof(Vertex), &obj->mesh->vertices[0], GL_STATIC_DRAW);

       //Create EBO
    glGenBuffers(1, &obj->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->indexCount * sizeof(unsigned int), &obj->mesh->indices[0], GL_STATIC_DRAW);

    //Set vertex attribute pointers
    SetVertexAttributePointers();

    obj->material = new Material();
    obj->material->SetShaders(fallback_VShader, simpleLit_FShader);
    obj->material->SetDiffuseTexture(containerTexture);
    obj->mesh->SetDrawMode(DrawMode::TRIANGLES);

    obj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    obj->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    litSceneObjects.push_back(obj);

    return true;

}

bool Engine::CreateCube()
{

     SceneObject *obj = new SceneObject();
    obj->mesh = new Mesh();
    obj->material = new Material();

    ReadOBJ((projectDirectory + cubeMeshPath).c_str(), obj->mesh->vertices, obj->mesh->indices, obj->name);

    obj->mesh->useEBO = true;

    obj->mesh->indexCount = static_cast<unsigned int>(obj->mesh->indices.size());

    glGenVertexArrays(1, &obj->mesh->VAO);
    glBindVertexArray(obj->mesh->VAO);

    glGenBuffers(1, &obj->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, obj->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->mesh->vertices.size() * sizeof(Vertex), &obj->mesh->vertices[0], GL_STATIC_DRAW);

       //Create EBO
    glGenBuffers(1, &obj->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->indexCount * sizeof(unsigned int), &obj->mesh->indices[0], GL_STATIC_DRAW);

    //Set vertex attribute pointers
    SetVertexAttributePointers();

    obj->material = new Material();
    obj->material->SetShaders(fallback_VShader, simpleLit_FShader);
    obj->material->SetDiffuseTexture(containerTexture);
    obj->mesh->SetDrawMode(DrawMode::TRIANGLES);

    obj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    obj->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    litSceneObjects.push_back(obj);

    return true;
}

bool Engine::CreateSceneObject(const char* objPath)
{

    SceneObject *obj = new SceneObject();

    obj->mesh = new Mesh();

    ReadOBJ(objPath, obj->mesh->vertices, obj->mesh->indices, obj->name);

    if (obj->name == std::string(""))
    {
        obj->name = "Scene Object";
    }

    obj->mesh->useEBO = true;

    obj->mesh->indexCount = static_cast<unsigned int>(obj->mesh->indices.size());

    glGenVertexArrays(1, &obj->mesh->VAO);
    glBindVertexArray(obj->mesh->VAO);

    //Create and bind VBO
    glGenBuffers(1, &obj->mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, obj->mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->mesh->vertices.size() * sizeof(Vertex), &obj->mesh->vertices[0], GL_STATIC_DRAW);

    //Create EBO
    glGenBuffers(1, &obj->mesh->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->mesh->indexCount * sizeof(unsigned int), &obj->mesh->indices[0], GL_STATIC_DRAW);

    //Set vertex attribute pointers
    SetVertexAttributePointers();

    obj->material = new Material();
    obj->material->SetShaders(fallback_VShader, simpleLit_FShader);
    obj->material->SetDiffuseTexture(containerTexture);
    obj->mesh->SetDrawMode(DrawMode::TRIANGLES);

    obj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    obj->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    litSceneObjects.push_back(obj);

    return true;
}

#pragma endregion

void Engine::ClearSceneObjects()
{

    renderInspectorWindow = false;
    selectedSceneObject = NULL;

    for (unsigned int i = 0; i < sceneObjects.size(); i++)
    {
        delete sceneObjects[i];
    }
    for (int i = 0; i < litSceneObjects.size(); i++)
    {
        delete litSceneObjects[i];
    }
    sceneObjects.clear();
    litSceneObjects.clear();
    renderSceneObjects = true;
}

void Engine::ProcessLit()
{
      for (unsigned int i = 0; i < litSceneObjects.size(); i++)
      {
        int ambientColorLocation = glGetUniformLocation(litSceneObjects[i]->material->shaderProgram, "ambientColor");
        glUseProgram(litSceneObjects[i]->material->shaderProgram);
        glUniform3f(ambientColorLocation, ambientColor.x, ambientColor.y, ambientColor.z);

        int ambientStrengthLocation = glGetUniformLocation(litSceneObjects[i]->material->shaderProgram, "ambientStrength");
        glUseProgram(litSceneObjects[i]->material->shaderProgram);
        glUniform1f(ambientStrengthLocation, ambientStrength);

        int lightPosLocation = glGetUniformLocation(litSceneObjects[i]->material->shaderProgram, "lightPos");
        glUseProgram(litSceneObjects[i]->material->shaderProgram);
        glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

        int lightColorLocation = glGetUniformLocation(litSceneObjects[i]->material->shaderProgram, "lightColor");
        glUseProgram(litSceneObjects[i]->material->shaderProgram);
        glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);

        int lightStrengthLocation = glGetUniformLocation(litSceneObjects[i]->material->shaderProgram, "lightStrength");
        glUseProgram(litSceneObjects[i]->material->shaderProgram);
        glUniform1f(lightStrengthLocation, lightStrength);

        litSceneObjects[i]->rotation.y += deltaTime * 20.0f;
        if (litSceneObjects[i]->rotation.y >= 360.0f)
        {
            litSceneObjects[i]->rotation.y = 0.0f;
        }
        
        //object->transform = glm::rotate(object->transform, glm::radians(float(glfwGetTime()) * -10.0f), glm::vec3(0.0, 1.0, 0.0));

        renderer.Render(litSceneObjects[i]);
      }
}

void Engine::ProcessUnlit()
{
    for (unsigned int i = 0; i < sceneObjects.size(); i++)
      {
        renderer.Render(sceneObjects[i]);
      }
}

void Engine::ProcessUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (renderDebugWindow)
    {
        CreateDebugSettingsWindow();
    }

    if (renderSceneList)
    {
        CreateSceneListWindow();
    }

    if (renderInspectorWindow)
    {
        CreateInspectorWindow();
    }


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// PUBLIC

bool Engine::Initialize()
{

    #ifdef Platform_Linux
        if (!CreateWindow(width,  height,  linuxTitle))
        {
            std::cout << "Failed to create window!" << std::endl;
            return false;
        }
    #endif

    #ifdef Platform_Apple
        if (!CreateWindow(width,  height, appleTitle))
        {
            std::cout << "Failed to create window!" << std::endl;
            return false;
        }

    #endif

    renderer = Graphics(width, height);
    renderer.SetFOV(60.0f);

    if (!renderer.Initialize(window))
    {
        std::cout << "Failed to initialize renderer!" << std::endl;
        return false;
    }

    camera = Camera();
    camera.Setup(glm::vec3(0.0f, 1.0f, 5.0f), window);
    

    std::cout << projectDirectory << std::endl;

    //create shaders
    CreateVertexShader(fallback_VShader, (projectDirectory + fallbackVertexPath).c_str());
    CreateVertexShader(grid_VShader, (projectDirectory + gridVertexPath).c_str());
    CreateFragmentShader(fallback_FShader, (projectDirectory + fallbackFragmentPath).c_str());
    CreateFragmentShader(grid_FShader, (projectDirectory + gridFragmentPath).c_str());
    CreateFragmentShader(simpleLit_FShader, (projectDirectory + simpleLitFragmentPath).c_str());

    //create textures
    CreateTexture((projectDirectory + containerTexturePath).c_str(), containerTexture);

    //create Grid
    CreateGrid();
    CreateRays();
  
    //Setup ImGui
    ImGuiSetup();

    return true;
}

void Engine::Loop()
{
    while (glfwWindowShouldClose(window) == false)
    {

        glfwPollEvents();

        CalculateDelta();

        ProcessInput();

        camera.Update(deltaTime);

        renderer.UpdateViewMatrix(camera.View);

        renderer.UpdateViewPosition(camera.GetPosition());

        renderer.ClearScreen(clearScreenColor.x, clearScreenColor.y, clearScreenColor.z);

        if (renderGrid)
        {
            renderer.Render(grid);
        }

        renderer.Render(xRay);
        renderer.Render(yRay);
        renderer.Render(zRay);

        if (renderSceneObjects)
        {

            ProcessLit();
            ProcessUnlit();
        }

        ProcessUI();

        glfwSwapBuffers(window);
    }
}

bool Engine::Shutdown()
{
    std::cout << "Shutting down engine..." << std::endl;

    renderDebugWindow = false;
    renderInspectorWindow = false;
    renderSceneList = false;
    selectedSceneObject = NULL;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteShader(fallback_VShader);
    glDeleteShader(fallback_FShader);
    glDeleteShader(grid_VShader);
    glDeleteShader(grid_FShader);
    glDeleteShader(simpleLit_FShader);

    delete grid;
    delete xRay;
    delete yRay;
    delete zRay;

    //delete renderer;

    ClearSceneObjects();

    glfwDestroyWindow(window);
    return true;
}
