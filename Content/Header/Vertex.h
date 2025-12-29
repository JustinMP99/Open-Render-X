#include <glm/glm.hpp>


class Vertex
{
  public:

  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 uv;

  Vertex();

  Vertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec2& tex)
    : position(pos), color(col), uv(tex)
  {
  }

};

