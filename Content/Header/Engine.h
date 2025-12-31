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
GLuint VAO;
GLuint VBO;
GLuint EBO;
std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

//Temp object material data
GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;

const char* windowsTitle = "Open Render X - OpenGL - Windows";
const char* appleTitle = "Open Render X - OpenGL - MacOS";

//File Paths

//MacOS


//Windows

private:

std::string LoadShaderAsString(const std::string& filename);

bool CreateTriangle();

bool CreateWindow(int width, int height, const char* title);

public:

bool Initialize();

void Loop();

bool Shutdown();

};