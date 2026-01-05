//
// Created by Justin Philie on 1/4/26.
//

#include "../Header/Material.h"

#include "glad/glad.h"


Material::Material()
{

}

Material::~Material()
{
    glDeleteProgram(shaderProgram);
}
