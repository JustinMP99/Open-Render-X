#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Graphics.h"
#include "Vertex.h"

class Engine
{

private:
GLFWwindow* window; ///< Pointer to the window that is created by GLFW
unsigned int width = 1290;;///< The width of the window
unsigned int height = 720; ///< The height of the window

Graphics renderer;

//Temp object mesh data
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;
std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

//Temp object material data
GLuint shaderProgram;
GLuint fallback_VShader;
GLuint fallback_FShader;

const char* windowsTitle = "Open Render X - OpenGL - Windows";
const char* appleTitle = "Open Render X - OpenGL - MacOS";

//File Paths

//MacOS
const char* fallbackVertexPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert"; ///< The path to the fallback vertex shader on macOS
    const char* fallbackFragmentPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag"; ///< The path to the fallback fragment shader on macOS

//Windows

private:

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

public:

    /// Initializes engine by creating window, renderer, shaders, textures and any desired objects
bool Initialize();

    /// Where the rendering and input handling occurs
void Loop();

    /// Releases all memory taken by the program
bool Shutdown();

};