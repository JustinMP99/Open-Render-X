//
// Created by Justin Philie on 1/3/26.
//

#include "../Header/SceneObject.h"

SceneObject::SceneObject()
{

}

SceneObject::~SceneObject()
{
    delete mesh;
    glDeleteProgram(shaderProgram);
}
