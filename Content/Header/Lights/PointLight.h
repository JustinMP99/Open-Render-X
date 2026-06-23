//
// Created by Justin Philie on 6/23/26.
//

#ifndef OPENRENDERX_POINTLIGHT_H
#define OPENRENDERX_POINTLIGHT_H
#include "Light.h"


class PointLight : public Light
{

public:

    float constant;
    float linear;
    float quadratic;
};


#endif //OPENRENDERX_POINTLIGHT_H
