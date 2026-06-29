//
// Created by Justin Philie on 6/25/26.
//

#ifndef OPENRENDERX_LIGHTS_H
#define OPENRENDERX_LIGHTS_H
#include "glm/vec3.hpp"

struct Light
{
    glm::vec3 color;
    float strength; // the strength of the light
};

struct DirectionalLight : public Light
{
    glm::vec3 direction; //the direction of the directional light
};

struct PointLight : public Light
{
    //glm::vec3 direction;
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight : public Light
{
    glm::vec3 direction;
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    float minCutoff;
    float maxCutoff;
};


#endif //OPENRENDERX_LIGHTS_H
