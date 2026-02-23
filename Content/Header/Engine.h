

#include "SceneObject.h"
#include "Graphics.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>


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

//Vertex Shaders
unsigned int fallback_VShader;

//Fragment Shaders
unsigned int fallback_FShader;

//Textures
unsigned int containerTexture;

const char* windowsTitle = "Open Render X - OpenGL - Windows";
const char* appleTitle = "Open Render X - OpenGL - MacOS";

//File Paths

//MacOS
    //const char* fallbackVertexPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    //const char* fallbackFragmentPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS
    //const char* containerTexturePath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Additional/Images/container.jpg"; ///< The path to the crate texture on macOS

//Linux - Laptop

    const char* fallbackVertexPath = "/run/media/jphilie/SSD_02/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    const char* fallbackFragmentPath = "/run/media/jphilie/SSD_02/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS
    const char* containerTexturePath = "/run/media/jphilie/SSD_02/Projects/Graphics/Open-Render-X/Content/Additional/Images/container.jpg"; ///< The path to the crate texture on macOS


//Windows - Laptop
    //const char* fallbackVertexPath = "E:/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    //const char* fallbackFragmentPath = "E:/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS
    //const char* containerTexturePath = "E:/Projects/Graphics/Open-Render-X/Content/Additional/Images/container.jpg"; ///< The path to the crate texture on macOS

//Windows - Desktop
    // const char* fallbackVertexPath = "D:/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    // const char* fallbackFragmentPath = "D:/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS
    // const char* containerTexturePath = "D:/Projects/Graphics/Open-Render-X/Content/Additional/Images/container.jpg";

private:

    /// Process basic input
    void ProcessInput();

    ///Loads the passed in shader file into a string to be compiled for use in rendering
    std::string LoadShaderAsString(const std::string& filename);

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

    /// Creates the window for the render engine
    bool CreateWindow(int width, int height, const char* title);

    /// Sets the vertex attribute pointers to fully utilize the Vertex data structure
    void SetVertexAttributePointers();

public:

    /// Initializes engine by creating window, renderer, shaders, textures and any desired objects
    bool Initialize();

    /// Where the rendering and input handling occurs
    void Loop();

    /// Releases all memory taken by the program
    bool Shutdown();

};