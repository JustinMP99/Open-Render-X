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
    delete material;
}


void SceneObject::Update(float delta)
{

    //==============================================================================================

    if (sinSlide)
    {
        sinMoveVal += delta * sinSlideSpeed;
        position.x = sin(sinMoveVal) * sinSlideFrequency;
    }

    if (rotate)
    {
        rotation.y += delta * 20.0f;
        if (rotation.y >= 360.0f)
        {
            rotation.y = 0.0f;
        }
    }
    
    //==============================================================================================

    //Update Transforms
    //reset the objects transform to identity before applying position, rotation, and scale
    transform = glm::mat4(1.0f);

    transform = glm::translate(transform, position);
    transform = glm::scale(transform, scale);
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.y),  glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    

    //use material
    material->Use();

}