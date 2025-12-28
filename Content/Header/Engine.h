#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"

class Engine
{

private:
GLFWwindow* window;
unsigned int width = 640;
unsigned int height = 480;

Graphics graphics;

GLuint VAO;
GLuint VBO;
GLuint EBO;



bool CreateWindow(int width, int height, const char* title);

public:

bool Initialize();

void Loop();

bool Shutdown();

};