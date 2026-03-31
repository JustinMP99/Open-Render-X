#version 330

in vec2 texCoord;
in vec3 normal;

in vec3 fragPos;

out vec4 FragColor;

//Textures 
uniform sampler2D diffuseTexture;

//Lighting
uniform vec3 ambientColor;
uniform float ambientStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightStrength;


vec3 calculateAmbient()
{
    return ambientColor * ambientStrength;
}

void main()
{

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0f);

    //calculate diffuse
    vec3 diffuse = diff * lightColor;

    diffuse *= lightStrength;

    //calculate ambient 
    vec3 ambient = calculateAmbient();

    //Return sampled texture
    vec4 texColor = texture(diffuseTexture, texCoord);

    // vec3 result = (ambient + diffuse) * texColor.rgb;
    vec3 result = ambient * diffuse * texColor.rgb;

    FragColor = vec4(result, 1.0f);

}