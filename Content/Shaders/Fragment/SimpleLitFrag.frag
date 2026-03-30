#version 330

in vec2 texCoord;
in vec3 normal;

out vec4 FragColor;

//Textures 
uniform sampler2D diffuseTexture;

//Lighting
uniform vec3 ambientColor;
uniform float ambientStrength;

void main()
{

    //calculate ambient 
    vec3 ambient = ambientColor * ambientStrength;

    //Return sampled texture
    FragColor = texture(diffuseTexture, texCoord);

    FragColor = vec4(FragColor.rgb * ambient, 1.0f);

}