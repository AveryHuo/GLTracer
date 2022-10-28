#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 TexCoord;
out vec3 LightingColor;

uniform mat4 model;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));

	  // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    LightingColor = ambient + diffuse + specular;
}