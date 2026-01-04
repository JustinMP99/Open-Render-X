//
// Created by Justin Philie on 1/4/26.
//

#include "../Header/Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    DestroyMeshData();
}

void Mesh::AddMeshData(std::vector<Vertex> &newVertices, std::vector<unsigned int> &newIndices)
{

    DestroyMeshData();

    //Copy vertices
    for (int i = 0; i < newVertices.size(); i++)
    {
        vertices.push_back(newVertices[i]);
    }

    //Copy indices
    for (int i = 0; i < newIndices.size(); i++)
    {
        indices.push_back(newIndices[i]);
    }

    //create vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create vbo
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //create ebo
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //set vertex attribute layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

}

void Mesh::DestroyMeshData()
{
    //clear vertex & index array
    vertices.clear();
    indices.clear();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
