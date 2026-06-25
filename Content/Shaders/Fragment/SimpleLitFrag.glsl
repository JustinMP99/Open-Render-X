#version 330

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 4

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

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float strength;
};

struct PointLight
{
    vec3 position;
    vec3 color;

    float strength;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 rotation;
    vec3 position;
    vec3 color;

    float strength;
    float constant;
    float linear;
    float quadratic;
    float minCutoff;
    float maxCutoff;
};

//Lights

uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

//Uniforms
uniform Material material;
uniform vec3 viewPos; //camera position
uniform bool useDiffuse;
uniform bool useSpecular;

//local variables
vec3 finalOutput;
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
      return ndotl * dirLight.color * 1.0f;
//    return ndotl * light.color * light.strength;
}

vec3 calculateSpecular(vec3 lightDir, vec3 norm, vec3 lightColor)
{
    vec4 specSample = texture(material.specularTexture, fs_in.texCoord);
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    return lightColor * (spec * specSample.rgb);
    //return light.color * (spec * vec3(1.0));
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{

    //calculate the normalized direction of the light
    vec3 lightDir = normalize(-light.direction); //calculation based on directional light direction

    //calculate dot product of the normal and light direction
    ndotl = max(dot(normal, lightDir), 0.0f);

    //calculate diffuse
    diffuse = ndotl * light.color * light.strength;

    //calculate specular
    //vec4 specSample = texture(material.specularTexture, fs_in.texCoord);
    //float specularStrength = 0.5f;
    vec3 reflectDir = reflect(-light.direction, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);
    specular = light.color * (spec * texture(material.specularTexture, fs_in.texCoord).rgb);

//    if(useSpecular)
//    {
//        specular = calculateSpecular(lightDir, normal, light.color);
//    }
//    else
//    {
//        specular = vec3(0.0f);
//    }

    //calculate ambient
    ambient = ambientColor.rgb * ambientStrength;

    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    //calculate the normalized direction of the light
    vec3 lightDir = normalize(-light.position - fragPos); //calculation based on directional light direction

    //calculate dot product of the normal and light direction
    ndotl = max(dot(normal, lightDir), 0.0f);

    //calculate diffuse
    diffuse = ndotl * light.color * light.strength;

    //calculate specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);
    specular = light.color * (spec * texture(material.specularTexture, fs_in.texCoord).rgb);

    //calculate ambient
    ambient = ambientColor.rgb * ambientStrength;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / ((light.constant + light.linear * distance + light.quadratic * (distance * distance)));

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    //calculate the normalized direction of the light
    vec3 lightDir = normalize(-light.position - fragPos); //calculation based on directional light direction

    //calculate dot product of the normal and light direction
    ndotl = max(dot(normal, lightDir), 0.0f);

    //calculate diffuse
    diffuse = ndotl * light.color * light.strength;

    //calculate specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);
    specular = light.color * (spec * texture(material.specularTexture, fs_in.texCoord).rgb);

    //calculate ambient
    ambient = ambientColor.rgb * ambientStrength;

    float theta = dot(lightDir, normalize(-lightDir));
    float epsilon = light.minCutoff - light.maxCutoff;
    float intensity = clamp((theta -  light.maxCutoff) / epsilon, 0.0f, 1.0f);

    diffuse *= intensity;
    specular *= intensity;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / ((light.constant + light.linear * distance + light.quadratic * (distance * distance)));

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{

    //calculate view direction
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);

    //normalize normal
    vec3 norm = normalize(fs_in.normal);

    finalOutput = CalculateDirectionalLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if(pointLights[i].color == vec3(0.0f))
        {
            continue;
        }
        finalOutput += CalculatePointLight(pointLights[i], norm, fs_in.fragPos, viewDir);
    }

    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        if(spotLights[i].color == vec3(0.0f))
        {
            continue;
        }
        finalOutput += CalculateSpotLight(spotLights[i], norm, fs_in.fragPos, viewDir);
    }

    if(useDiffuse)
    {
        texColor = texture(material.diffuseTexture, fs_in.texCoord);
    }
    else
    {
        texColor = vec4(1.0f);
    }

    finalOutput *= texColor.rgb;

    //Return sampled texture
    FragColor = vec4(finalOutput, 1.0f);

}