#version 330

out vec4 FragColor;

//Textures 
uniform sampler2D diffuseTexture;

//Lighting
uniform vec3 ambientColor;
uniform float ambientStrength;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightStrength;

//camera view position
uniform vec3 viewPos;

//Interface block
in VS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
}fs_in;


vec3 calculateAmbient()
{
    return ambientColor * ambientStrength;
}

vec3 calculateDiffuse(float diff)
{
    return diff * lightColor * lightStrength;   
}

vec3 calculateSpecular(vec3 lightDir, vec3 norm)
{
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    return specularStrength * spec * lightColor;
}

void main()
{

    vec3 norm = normalize(fs_in.normal);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);

    float diff = max(dot(norm, lightDir), 0.0f);

    //calculate diffuse
    vec3 diffuse = calculateDiffuse(diff);

    //calculate ambient 
    vec3 ambient = calculateAmbient();

    //specular
    vec3 specular = calculateSpecular(lightDir, norm);

    //Return sampled texture
    vec4 texColor = texture(diffuseTexture, fs_in.texCoord);

    // vec3 result = (ambient + diffuse) * texColor.rgb;
    vec3 result = (ambient + diffuse + specular) * texColor.rgb;

    FragColor = vec4(result, 1.0f);

}