#version 330 core
// struct Material {
//     sampler2D diffuse;
//     sampler2D specular;
//     float shininess;
// }; 

// struct DirLight {
//     vec3 direction;
	
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// struct PointLight {
//     vec3 position;
    
//     float constant;
//     float linear;
//     float quadratic;
	
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

out vec4 FragColor;
in vec2 TexCoor;

uniform vec4 color;
uniform sampler2D t;
uniform float mode;

// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    FragColor = mode * texture(t, TexCoor) + (1 - mode) * color;
}

// // Calculates the color when using a directional light.
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     // Diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // Specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // Combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     return (ambient + diffuse + specular);
// }

// // Calculates the color when using a point light.
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // Diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // Specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // Attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//     // Combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// }

// // Calculates the color when using a spot light.
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // Diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // Specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // Attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//     // Spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction)); 
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//     // Combine results
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;
//     return (ambient + diffuse + specular);
// }