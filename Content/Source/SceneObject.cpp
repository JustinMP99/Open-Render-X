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


void SceneObject::Update()
{

    //Update Transforms
    //reset the objects transform to identity before applying position, rotation, and scale
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, scale);
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    
    //object->transform = glm::rotate(object->transform, glm::radians(float(glfwGetTime()) * -10.0f), glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, glm::radians(rotation.y),  glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //use material
    material->Use();

    //==============================================================================================

    /*Do own code here*/

    //==============================================================================================
}