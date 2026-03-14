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
  glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
  float fov = 60.0f;

  void SetModel(SceneObject* object);

  void UpdateTransformUniforms(SceneObject* object);

public:

    Graphics();
    Graphics(int width, int height);
    ~Graphics();

    bool Initialize(GLFWwindow* win);
    void Render();
    void Render(SceneObject* object);
    bool Shutdown();
    void ClearScreen();
    void UpdateViewMatrix(glm::mat4 viewMatrix);
    void SetFOV(float fov);
};
