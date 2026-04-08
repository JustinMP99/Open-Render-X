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
    usingDiffuse = false;
    shininess = 32.0f;
}

Material::~Material()
{
    glDeleteProgram(shaderProgram);
}

void Material::Use()
{
    
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    SetInt("diffuseTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    SetInt("specularTexture", 1);

    glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), shininess);

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
    return true;
}

bool Material::SetDiffuseTexture(unsigned int diffuse)
{
    usingDiffuse = true;
    diffuseTexture = diffuse;
    return true;
}

bool Material::SetSpecularTexture(unsigned int specular)
{
    usingSpecular = true;
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
