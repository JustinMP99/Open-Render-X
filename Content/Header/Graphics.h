
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Graphics
{
  private:

  GLFWwindow* window;
  int width;
  int height;

  public:

  bool Initialize(GLFWwindow* win);

  void Render();

  bool Shutdown();
    

};