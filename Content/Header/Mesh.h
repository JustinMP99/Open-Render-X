
#include <vector>

#ifndef OPENRENDERX_VERTEX_H
#define OPENRENDERX_VERTEX_H
#include "Vertex.h"
#endif

class Mesh
{
    private:

    public:
    unsigned int VAO; ///< Vertex Array Object
    unsigned int VBO; ///< Vertex Buffer Object - Stores mesh vertex data for use with OpenGL
    unsigned int EBO; ///< Element Buffer Object - Stores mesh index data for use with OpenGL
    std::vector<Vertex> vertices; ///< Vector that stores the Vertex data for each vertice in the Mesh
    std::vector<unsigned int> indices; ///< Vector that stores the Index data for the Mesh

    Mesh();
    ~Mesh();

    /// Resets mesh data using the passed in vertices and indices data. Buffers are freed from memory and are recreated using new data as well.
    void AddMeshData(std::vector <Vertex> &vertices, std::vector <unsigned int> &indices);

    /// Frees all mesh data (VAO, VBO, EBO, etc.)
    void DestroyMeshData();

};
