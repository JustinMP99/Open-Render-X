#version 330

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D diffuseTexture;

//Interface block
in VS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
}fs_in;


void main()
{

    //Return simple color
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

}
