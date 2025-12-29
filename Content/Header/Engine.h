#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Graphics.h"
#include "Vertex.h"

class Engine
{

private:
GLFWwindow* window;
unsigned int width = 640;
unsigned int height = 480;

Graphics graphics;

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

private:

bool CreateTriangle();

bool CreateWindow(int width, int height, const char* title);

public:

bool Initialize();

void Loop();

bool Shutdown();

};