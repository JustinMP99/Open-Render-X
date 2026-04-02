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

void main()
{

    vec3 norm = normalize(fs_in.normal);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);

    float diff = max(dot(norm, lightDir), 0.0f);

    //calculate diffuse
    vec3 diffuse = diff * lightColor;

    diffuse *= lightStrength;

    //calculate ambient 
    vec3 ambient = calculateAmbient();

    //Return sampled texture
    vec4 texColor = texture(diffuseTexture, fs_in.texCoord);

    // vec3 result = (ambient + diffuse) * texColor.rgb;
    vec3 result = (ambient + diffuse) * texColor.rgb;

    FragColor = vec4(result, 1.0f);

}