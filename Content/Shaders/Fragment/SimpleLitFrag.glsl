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

//Directional light
uniform vec3 direction;
uniform vec3 color;

//Point Light
uniform float constant;
uniform float linear;
uniform float quadratic;

//Uniforms
uniform Material material;
uniform LightData light;
uniform vec3 viewPos; //camera position
uniform bool useDiffuse;
uniform bool useSpecular;

//local variables
float ndotl;
vec3 diffuse;
vec3 specular;
vec3 ambient;
vec4 texColor;

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
    //return light.color * (spec * vec3(1.0));
}

void main()
{
   
   if(useDiffuse)
   {
        texColor = texture(material.diffuseTexture, fs_in.texCoord);
   }
   else
   {
        texColor = vec4(1.0f);
   }
   
    //normalize normal
    vec3 norm = normalize(fs_in.normal);

    //calculate the normalized direction of the light
    vec3 lightDir = normalize(-direction); //calculation based on directional light direction
    //vec3 lightDir = normalize(light.position - fs_in.fragPos); //calculation based on light position and frag position
    //calculate dot product of the normal and light direction
    ndotl = max(dot(norm, lightDir), 0.0f);

    //calculate diffuse
    diffuse = calculateDiffuse();

    //calculate ambient 
    ambient = calculateAmbient();

    float distance = length(light.position - fs_in.fragPos);
    float attenuation = 1.0f / ((constant + linear * distance + quadratic * (distance * distance)));

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    //specular
    if(useSpecular)
    {
        specular = calculateSpecular(lightDir, norm);
    }
    else
    {
        specular = vec3(0.0f);
    }
   
     vec3 result;
    //Return sampled texture
    result = (ambient + diffuse + specular) * texColor.rgb;

    FragColor = vec4(result, 1.0f);

}