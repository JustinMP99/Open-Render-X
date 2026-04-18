//
// Created by Justin Philie on 1/4/26.
//

#include "../Header/Material.h"

std::string GetFileContents(const char* filename)
{
	std::string result = "";
	std::string line = "";
	std::ifstream in(filename);
	if (in.is_open())
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

/// PUBLIC

Material::Material()
{
    shininess = 32.0f;
    shaderProgram = 0;
    diffuseTexture = 0;
    specularTexture = 0;
    useDiffuse = true;
    useSpecular = true;
}

Material::~Material()
{
    glDeleteProgram(shaderProgram);
}

void Material::Use()
{
    if (shaderProgram != 0)
    {
        if (shaderProgram != 0)
        {
            glUseProgram(shaderProgram);
        }
        
        if (diffuseTexture != 0 && useDiffuse)
        {
            SetInt("material.diffuseTexture", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        }
        
        if (specularTexture != 0 && useSpecular)
        {
            SetInt("material.specularTexture", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTexture);
        }
        
        SetFloat("material.shininess", shininess);
    }
}

bool Material::SetMaterialName(const char* name)
{
    MaterialName = std::string(name);
    return true;
}

bool Material::SetShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // Bind uniform blocks to binding points
    GLuint ambientBlockIndex = glGetUniformBlockIndex(shaderProgram, "AmbientLightData");
    if (ambientBlockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(shaderProgram, ambientBlockIndex, 1);
    }

    return true;
}

bool Material::SetDiffuseTexture(unsigned int diffuse)
{
    diffuseTexture = diffuse;
    return true;
}

bool Material::SetSpecularTexture(unsigned int specular)
{
    specularTexture = specular;
    return true;
}

bool Material::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
	return true;
}

bool Material::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	return true;
}

bool Material::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	return true;
}

bool Material::SetVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
	return true;
}