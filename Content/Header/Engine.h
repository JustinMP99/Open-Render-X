#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Engine
{

private:
GLFWwindow* window;
unsigned int width = 640;
unsigned int height = 480;

bool CreateWindow(int width, int height, const char* title);

public:

bool Initialize();

void Loop();

bool Shutdown();


};