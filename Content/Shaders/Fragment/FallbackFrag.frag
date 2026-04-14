#version 330

//Interface block
in VS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
}fs_in;

//DATA STRUCTS
struct Material
{
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    float shininess;
};

out vec4 FragColor;

void main()
{

    //Return simple color
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

}
