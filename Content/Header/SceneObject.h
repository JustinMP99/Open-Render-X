//
// Created by Justin Philie on 1/3/26.
//

#include <vector>
// #include "Vertex.h"

class SceneObject
{

private:


public:
    //Temp object mesh data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    //Temp object material data
    GLuint shaderProgram;

};