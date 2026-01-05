#pragma once

#include "SceneObject.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
