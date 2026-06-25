//
// Created by Justin Philie on 6/23/26.
//

#ifndef OPENRENDERX_SPOTLIGHT_H
#define OPENRENDERX_SPOTLIGHT_H
#include "Light.h"

class SpotLight : public Light
{

    public:

    float constant;
    float linear;
    float quadratic;

    float minCutoff;
    float maxCutoff;

};

#endif //OPENRENDERX_SPOTLIGHT_H
