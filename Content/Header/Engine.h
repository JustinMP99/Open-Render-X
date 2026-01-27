#include "SceneObject.h"
#include "Graphics.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>


#define GL_SILENCE_DEPRICATIONßßß

class Engine
{

private:
GLFWwindow* window; ///< Pointer to the window that is created by GLFW
unsigned int width = 1290;;///< The width of the window
unsigned int height = 720; ///< The height of the window

Graphics renderer;

    std::vector<SceneObject*> sceneObjects;

    //Vertex Shaders
    GLuint fallback_VShader;

    //Fragment Shaders
    GLuint fallback_FShader;

    const char* windowsTitle = "Open Render X - OpenGL - Windows";
    const char* appleTitle = "Open Render X - OpenGL - MacOS";

//File Paths

//MacOS
    const char* fallbackVertexPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    const char* fallbackFragmentPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS

//Windows

private:

    /// Process basic input
    void ProcessInput();

    ///Loads the passed in shader file into a string to be compiled for use in rendering
    std::string LoadShaderAsString(const std::string& filename);

    /// Creates a vertex shader using the passed in shader and shader path
    bool CreateVertexShader(unsigned int &shader, const char* shaderPath);

    /// Creates a fragment shader using the passed in shader and shader path
    bool CreateFragmentShader(unsigned int &shader, const char* shaderPath);

    /// Creates a triangle to be rendered
    bool CreateTriangle();

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