#version 330

out vec4 FragColor;

uniform vec3 color;
//uniform sampler2D diffuseTexture;

void main()
{
    //Return simple color
    FragColor = vec4(color, 1.0f);
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

