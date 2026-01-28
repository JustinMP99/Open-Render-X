#version 330

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D diffuseTexture;

void main()
{

    //Return simple color
    //FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    //Return sampled texture
    FragColor = texture(diffuseTexture, texCoord);

}
