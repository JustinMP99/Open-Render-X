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

/**************************************************************************//**
 * The Materials class handles contains data related to shaders, textures and other rendering data
 ******************************************************************************/
class Material
{
private:

public:

    std::string MaterialName; ///< string that holds the name of the material
    unsigned int shaderProgram; ///< the id of the ShaderProgram
    unsigned int diffuseTexture; ///< the id of the attached diffuse texture
    unsigned int specularTexture; ///< the id of the specular texture
    float shininess; ///< value for shininess
    bool useDiffuse; ///< bool to determine if the diffuse texture should be used
    bool useSpecular; ///< bool to determine if the specular texture should be used

    Material();
    Material(unsigned int vertexShader, unsigned int fragmentShader);
    Material(unsigned int vertexShader, unsigned int fragmentShader, unsigned int diffuseTexture);
    ~Material();

    /// <summary>
    /// Activates the shader program and binds the textures associated with this material
    /// </summary>
    void Use();

    /// @brief stes the name of the material
    /// @param name string containing the name that will be given to the material
    /// @return true if successful
    bool SetMaterialName(const char* name);

    /// @brief creates the Shader Program and attaches and links the vertex and fragment shaders passed in
    /// @param vertexShader the id for the vertex shader 
    /// @param fragmentShader the id for the fragment shader
    /// @return true if successful
    bool SetShaders(unsigned int vertexShader, unsigned int fragmentShader);

    /// sets the diffuse texture on the material
    bool SetDiffuseTexture(unsigned int diffuse);

    /// sets the specular texture on the material
    bool SetSpecularTexture(unsigned int specular);

    //sets a bool uniforms value
    bool SetBool(const std::string &name, bool value) const;

    /// @brief sets the value of an int in the attached shader program
    /// @param name string that holds the name of the int uniform
    /// @param value the new value for the int
    /// @return true if successful
    bool SetInt(const std::string &name, int value) const;

    /// @brief sets the value of a float in the attached shader program
    /// @param name string that holds the name of the float uniform
    /// @param value the new value for the float
    /// @return true if successful
    bool SetFloat(const std::string &name, float value) const;

    /// @brief sets the value of a vec3 uniform in the attached shader program
    /// @param name string that holds the name of the vec3 uniform
    /// @param x x value of the vec3
    /// @param y y value of the vec3
    /// @param z z value of the vec3
    /// @return true if successful
    bool SetVec3(const std::string& name, float x, float y, float z) const;
};

#endif //OPENRENDERX_MATERIAL_H