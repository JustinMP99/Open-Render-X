//
// Created by Justin Philie on 6/24/26.
//

#ifndef OPENRENDERX_DISPLAYHANDLER_H
#define OPENRENDERX_DISPLAYHANDLER_H
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

enum DisplayMode
{
    GLFW,
    NATIVE
};

class DisplayHandler
{

private:

    unsigned int window_width;
    unsigned int window_height;
    GLFWwindow *window;
    std::vector<GLFWwindow *> windows;


    const char* windowsTitle = "Open Render X - OpenGL - Windows";
    const char* appleTitle = "Open Render X - OpenGL - MacOS";
    const char* linuxTitle = "Open Render X - OpenGL - Linux";

    DisplayMode displayMode;

    bool CreateGLFWWindow();

    bool CreateNativeWindow();

public:

    DisplayHandler();
    ~DisplayHandler();

    bool Initialize();

    bool CreateWindow();

    bool Shutdown();

    bool SwapBuffers();

    int ShouldClose();

    GLFWwindow *GetWindow();

    void SetWindowSize(unsigned int width, unsigned int height);

    void SetDisplayMode(DisplayMode mode);

};


#endif //OPENRENDERX_DISPLAYHANDLER_H
