#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Light
{


private:


public:

    glm::mat4 transform;
    glm::vec3 position;
    glm::vec3 rotation; //direction on spotlight
    glm::vec3 scale;


    glm::vec3 color;


};
