// Basic Texture Shader
 
 #type vertex
 #version 330 core
 
 layout(location = 0) in vec3 a_Position;
 layout(location = 1) in vec2 a_TexCoord;
 layout(location = 2) in vec3 a_Normal;
 
 uniform mat4 u_ViewProjection;
 uniform mat4 u_Transform;
 
 out vec2 v_TexCoord;
 out vec3 v_FragmentPosition;
 out vec3 v_Normal;
 
 void main()
 {
 	v_TexCoord = a_TexCoord;
    v_FragmentPosition = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
 	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
 }
 




 #type fragment
 #version 330 core
 
 layout(location = 0) out vec4 color;
 
 struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
 }; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

 in vec2 v_TexCoord;
 in vec3 v_FragmentPosition;
 in vec3 v_Normal;

 uniform vec4 u_Color;

 uniform vec3 u_CameraPosition;
 uniform Material material;

 #define MAX_LIGHTS 10
 uniform int u_NumDirLights;
 uniform int u_NumPointLights; // Actual number of active point lights
 uniform int u_NumSpotLights;

 uniform DirLight dirLights[MAX_LIGHTS];
 uniform PointLight pointLights[MAX_LIGHTS];
 uniform SpotLight spotLights[MAX_LIGHTS];

 // function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);

 void main()
 {
    // properties
    vec3 diffuseColor = (texture(material.diffuse, v_TexCoord) * u_Color).rgb;
    vec3 specularColor = (texture(material.specular, v_TexCoord) * u_Color).rgb;
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_CameraPosition - v_FragmentPosition);
    vec3 result = vec3(0.0);
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    for(int i = 0; i < u_NumDirLights; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir, diffuseColor, specularColor);
    // phase 2: point lights
    for(int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, v_FragmentPosition, viewDir, diffuseColor, specularColor);    
    // phase 3: spot light
    for(int i = 0; i < u_NumSpotLights; i++)
        result += CalcSpotLight(spotLights[i], norm, v_FragmentPosition, viewDir, diffuseColor, specularColor);

 	color = vec4(result, 1.0);
 }

 // calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}