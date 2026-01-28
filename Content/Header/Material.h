//
// Created by Justin Philie on 1/4/26.
//

#ifndef OPENRENDERX_MATERIAL_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#define OPENRENDERX_MATERIAL_H

/// <summary>
/// Reads the file passed in and returns its contents as a string
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
std::string GetFileContents(const char* filename);

class Material
{
private:

    public:
    std::string MaterialName;
    unsigned int shaderProgram;
    unsigned int diffuseTexture;
    unsigned int specularTexture;

    Material();
    Material(unsigned int vertexShader, unsigned int fragmentShader);
    Material(unsigned int vertexShader, unsigned int fragmentShader, unsigned int diffuseTexture);
    ~Material();

    /// <summary>
    /// Activates the shader program and binds the textures associated with this material
    /// </summary>
    void Use();

    bool SetMaterialName(const char* name);

    /// <summary>
    /// Creates the Shader Program and attaches and links the vertex and fragment shaders passed in
    /// </summary>
    bool SetShaders(unsigned int vertexShader, unsigned int fragmentShader);
    bool SetDiffuseTexture(unsigned int diffuese);
    bool SetBool(const std::string &name, bool value) const;
    bool SetInt(const std::string &name, int value) const;
    bool SetFloat(const std::string &name, float value) const;

};


#endif //OPENRENDERX_MATERIAL_H