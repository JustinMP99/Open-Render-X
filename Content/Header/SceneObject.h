//
// Created by Justin Philie on 1/3/26.
//

#include <vector>

#ifndef OPENRENDERX_MESH_H
#define OPENRENDERX_MESH_H
#include "Mesh.h"
#endif

#ifndef OPENRENDERX_VERTEX_H
#define OPENRENDERX_VERTEX_H
#include "Vertex.h"
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


public:

    Mesh* mesh; ///< Stores mesh data for the SceneObject

    //Temp object material data
    GLuint shaderProgram;

    SceneObject();
    ~SceneObject();

};