#version 330

//UNIFORM BUFFERS
layout (std140) uniform AmbientLightData
{
    vec4 ambientColor;
    float ambientStrength;
};

//INTERFACE BLOCK
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

struct LightData
{
    vec3 position;
    vec3 color;
    float strength;
};

//Uniforms
uniform Material material;
uniform LightData light;
uniform vec3 viewPos; //camera position

//local variables
float ndotl;
vec3 diffuse;
vec3 specular;
vec3 ambient;

//output value
out vec4 FragColor;

vec3 calculateAmbient()
{
    return ambientColor.rgb * ambientStrength;
}

vec3 calculateDiffuse()
{
    return ndotl * light.color * light.strength;   
}

vec3 calculateSpecular(vec3 lightDir, vec3 norm)
{
    vec4 specSample = texture(material.specularTexture, fs_in.texCoord);
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    return light.color * (spec * specSample.rgb);
}

void main()
{
    //Return sampled texture
    vec4 texColor = texture(material.diffuseTexture, fs_in.texCoord);

    //normalize normal
    vec3 norm = normalize(fs_in.normal);

    //calculate the normalized direction of the light
    // vec3 lightDir = normalize(lightPos - fs_in.fragPos); 
    vec3 lightDir = normalize(light.position - fs_in.fragPos); 
    //calculate dot product of the normal and light direction
    ndotl = max(dot(norm, lightDir), 0.0f);

    //calculate diffuse
    diffuse = calculateDiffuse();

    //calculate ambient 
    ambient = calculateAmbient();

    //specular
    specular = calculateSpecular(lightDir, norm);

    vec3 result = (ambient + diffuse + specular) * texColor.rgb;

    FragColor = vec4(result, 1.0f);

}