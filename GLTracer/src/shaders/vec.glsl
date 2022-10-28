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
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}