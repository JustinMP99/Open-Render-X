
#include <GLFW/glfw3.h>

class Graphics
{
  private:

  GLFWwindow* window;

  public:

  bool Initialize(GLFWwindow* win);

  void Render();

  bool Shutdown();
    

};