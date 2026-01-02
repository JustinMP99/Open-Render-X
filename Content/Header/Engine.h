#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

//#include "../Globals.h"
#include "Graphics.h"
#include "Vertex.h"

class Engine
{

private:
GLFWwindow* window;
unsigned int width = 1290;;
unsigned int height = 720;

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
const char* fallbackVertexPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Vertex/FallbackVertex.vert";
    const char* fallbackFragmentPath = "/Users/justinphilie/Documents/Projects/Graphics/Open-Render-X/Content/Shaders/Fragment/FallbackFrag.frag";

//Windows

private:

std::string LoadShaderAsString(const std::string& filename);

    bool CreateVertexShader(unsigned int &shader, const char* shaderPath);

    bool CreateFragmentShader(unsigned int &shader, const char* shaderPath);

bool CreateTriangle();

bool CreateWindow(int width, int height, const char* title);

public:

bool Initialize();

void Loop();

bool Shutdown();

};