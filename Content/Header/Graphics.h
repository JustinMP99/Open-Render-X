#pragma once

#include "SceneObject.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**************************************************************************//**
 * The Graphics class handles rendering of SceneObjects to the screen. It contains a pointer to the window where rendering occurs,
 * as well as methods for initializing the graphics context, rendering objects, and shutting down the graphics system.
 ******************************************************************************/
class Graphics
{
  private:

  GLFWwindow* window;
  int width;
  int height;

  public:

    Graphics();
    Graphics(int width, int height);
    ~Graphics();

    bool Initialize(GLFWwindow* win);

    void Render();
    void Render(SceneObject* object);

    bool Shutdown();
    

};
