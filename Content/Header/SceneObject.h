//
// Created by Justin Philie on 1/3/26.
//
#pragma once
#include <vector>

#include "Material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #ifndef OPENRENDERX_MATERIAL_H
// #define OPENRENDERX_MATERIAL_H
// #include "Material.h"
// #endif

#ifndef OPENRENDERX_MESH_H
#define OPENRENDERX_MESH_H
#include "Mesh.h"
#endif

/**************************************************************************//**
 * The SceneObject class stores all data related to object present in the renderer. This includes:
 *  - Mesh Data
 *  - Material Data
 *  - etc.
 ******************************************************************************/
class SceneObject
{

private:

    float sinMoveVal = 0.0f;

    /// @brief Updates transform matrix by applying position, scale and rotation vectors
    void UpdateTransforms();

public:

    std::string name; ///< The name of the SceneObject, used for organization and debugging purposes
	glm::mat4 transform;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

    Mesh* mesh; ///< Stores mesh data for the SceneObject
    Material* material; ///< Stores material data for SceneObject

    bool rotate = false;
    bool sinSlide = false;
    float sinSlideSpeed = 1.0f;
    float sinSlideFrequency = 1.0f;

    SceneObject();
    ~SceneObject();

    void Update(float delta);

};