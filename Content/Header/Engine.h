

#include "SceneObject.h"
#include "Graphics.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#define GL_SILENCE_DEPRICATION

class Engine
{

private:
GLFWwindow* window; ///< Pointer to the window that is created by GLFW
unsigned int width = 1290;///< The width of the window
unsigned int height = 720; ///< The height of the window

Graphics renderer;
Camera camera;

float deltaTime = 0.0f; ///< The time it takes to render a single frame
float lastFrame = 0.0f; ///< The time it took to render the last

std::vector<SceneObject*> sceneObjects;

//Grid Data
SceneObject *grid;
int gridHorizontal = 1000;
int gridVertical = 1000;

//Vertex Shaders
unsigned int fallback_VShader;
unsigned int grid_VShader;

//Fragment Shaders
unsigned int fallback_FShader;
unsigned int grid_FShader;

//Textures
unsigned int containerTexture;

const char* windowsTitle = "Open Render X - OpenGL - Windows";
const char* appleTitle = "Open Render X - OpenGL - MacOS";
const char* linuxTitle = "Open Render X - OpenGL - Linux";

//File Paths

std::string projectDirectory = std::filesystem::current_path().string(); ///< The path to the project directory, used as a base for all other file paths

#ifdef Platform_Apple

    std::string appleProjectDirectory = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/";

#endif

#ifdef Platform_Linux
   
    //Desktop
    std::string linuxProjectDirectory = "/mnt/1b8f20dd-88cc-4bd6-8bca-2f8c5e21dfba/Projects/Graphics/Open-Render-X/";
    
    //Laptop
    //std::string linuxProjectDirectory = "/mnt/03796401-d644-4d8d-8373-0614cdaf42a0/Projects/Graphics/Open-Render-X/";

#endif

    const char* fallbackVertexPath = "Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    const char* gridVertexPath = "Content/Shaders/Vertex/GridVertex.vert"; ///< The path to the grid vertex shader on macOS
    const char* fallbackFragmentPath = "Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS
    const char* gridFragmentPath = "Content/Shaders/Fragment/GridFrag.frag"; ///< The path to the grid fragment shader on macOS
    const char* containerTexturePath = "Content/Additional/Images/container.jpg"; ///< The path to the crate texture on macOS
    const char* objModelPath = "Content/Additional/mesh/cube.obj"; ///< The path to the OBJ model on macOS


private:

    /// Process basic input
    void ProcessInput();

    ///Loads the passed in shader file into a string to be compiled for use in rendering
    std::string LoadShaderAsString(const std::string& filename);

    void ReadOBJ(std::string filepath, std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices);

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

    bool CreateGrid();

    bool CreateSceneObject(const char* objPath);

    /// Creates the window for the render engine
    bool CreateWindow(int width, int height, const char* title);

    /// Sets the vertex attribute pointers to fully utilize the Vertex data structure
    void SetVertexAttributePointers();

    void CalculateDelta();

public:

    /// Initializes engine by creating window, renderer, shaders, textures and any desired objects
    bool Initialize();

    /// Where the rendering and input handling occurs
    void Loop();

    /// Releases all memory taken by the program
    bool Shutdown();

};