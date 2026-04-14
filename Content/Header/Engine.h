#include "SceneObject.h"
#include "Graphics.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#define GL_SILENCE_DEPRICATION

struct AmbientLightData
{

    glm::vec4 ambientColor;
    float ambientStrength;

};


class Engine
{

private:

    //Window Data
    GLFWwindow* window; ///< Pointer to the window that is created by GLFW
    unsigned int width = 1290;///< The width of the window
    unsigned int height = 720; ///< The height of the window

    //Additional Core Data
    Graphics renderer;
    Camera camera;

    float deltaTime = 0.0f; ///< The time it takes to render a single frame
    float lastFrame = 0.0f; ///< The time it took to render the last

    std::vector<SceneObject*> sceneObjects;
    std::vector<SceneObject*> litSceneObjects; ///< Vector that stores all SceneObjects that use the simple lit shader
    SceneObject* selectedSceneObject = nullptr; ///< Pointer to the currently selected SceneObject, used for the inspector window

    //Debug Settings
    bool renderSceneObjects = true;
    bool renderDebugWindow = true;
    bool renderSceneList = true;
    bool renderInspectorWindow = false;
    bool renderGrid = true;
    bool enableWireframe = false;
    glm::vec3 clearScreenColor = glm::vec3(0.0f, 0.0f, 0.0f);

    //Lighting Data
    unsigned int ambientLightUBO;
    AmbientLightData ambientLightData;
    glm::vec4 ambientColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    float ambientStrength = 1.0f;

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    float lightStrength = 1.0f;

    //Grid Data
    SceneObject *grid;
    SceneObject *xRay;
    SceneObject *yRay;
    SceneObject *zRay;
    int gridHorizontal = 1000;
    int gridVertical = 1000;

    //Vertex Shaders
    unsigned int fallback_VShader;
    unsigned int grid_VShader;

    //Fragment Shaders
    unsigned int fallback_FShader;
    unsigned int grid_FShader;
    unsigned int simpleLit_FShader;

    //Textures
    unsigned int containerTexture;
    unsigned int container_diffuse_texture;
    unsigned int container_specular_texture;

    const char* windowsTitle = "Open Render X - OpenGL - Windows";
    const char* appleTitle = "Open Render X - OpenGL - MacOS";
    const char* linuxTitle = "Open Render X - OpenGL - Linux";

    //File Paths

    std::string projectDirectory = PROJECT_DIR;

    //Shader Paths
    const char* fallbackVertexPath = "/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader
    const char* gridVertexPath = "/Content/Shaders/Vertex/GridVertex.vert"; ///< The path to the grid vertex shader
    const char* fallbackFragmentPath = "/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader
    const char* gridFragmentPath = "/Content/Shaders/Fragment/GridFrag.frag"; ///< The path to the grid fragment shader
    const char* simpleLitFragmentPath = "/Content/Shaders/Fragment/SimpleLitFrag.frag"; ///< The path to the simple lit fragment shader

    //Texture Paths
    const char* containerTexturePath = "/Content/Additional/Images/container.jpg"; ///< The path to the crate texture
    const char* container_diffuse = "/Content/Additional/Images/container2.png";
    const char* container_specular = "/Content/Additional/Images/container2_specular.png";

    //Model Paths
    const char* cubeMeshPath = "/Content/Additional/mesh/cube.obj"; ////< The path to the OBJ model
    const char* sphereMeshPath = "/Content/Additional/mesh/Sphere.obj"; ///< The path to the sphere OBJ model

private:

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    float GetRandomFloat(float min, float max);

    //PROGRAM LOOP FUNCTIONS

    /// Process basic input
    void ProcessInput();

    void CalculateDelta();

    void CreateDebugSettingsWindow();

    void CreateSceneListWindow();

    void CreateInspectorWindow();

    void UpdateAmbient();

    //SETUP UTILITY FUNCTIONS

    ///Loads the passed in shader file into a string to be compiled for use in rendering
    std::string LoadShaderAsString(const std::string& filename);

    void ReadOBJ(std::string filepath, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string &objectName);

    /// Creates a vertex shader using the passed in shader and shader path
    bool CreateVertexShader(unsigned int &shader, const char* shaderPath);

    /// Creates a fragment shader using the passed in shader and shader path
    bool CreateFragmentShader(unsigned int &shader, const char* shaderPath);

    /// Creates a texture using the passed in file path and assigns it to the passed in texture variable
    bool CreateTexture(const char* filepath, unsigned int &texture);

    /// Creates a triangle to be rendered
    bool CreateTriangle();

    /// Creates a quad to be rendered
    bool CreateQuad();

    bool CreateCube_Old();

    bool CreateSphere();

    bool CreateCube();

    bool CreateGrid();

    bool CreateRays();

    //Creates a SceneObject and fills it's mesh data by reading the passed in OBJ file path
    bool CreateSceneObject(const char* objPath);

    /// Creates the window for the render engine
    bool CreateWindow(int width, int height, const char* title);

    /// Sets the vertex attribute pointers to fully utilize the Vertex data structure
    void SetVertexAttributePointers();

    void ImGuiSetup();

    void ClearSceneObjects();

    /// Processes all lit SceneObjects in the scene using the simple lit shader
    void ProcessLit();

    void ProcessUnlit();

    void ProcessUI();

public:

    /// Initializes engine by creating window, renderer, shaders, textures and any desired objects
    bool Initialize();

    /// Where the rendering and input handling occurs
    void Loop();

    /// Releases all memory taken by the program
    bool Shutdown();

};
