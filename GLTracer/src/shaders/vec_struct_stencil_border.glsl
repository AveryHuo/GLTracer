#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aTangent;
layout (location = 4) in vec2 aBitangent;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	Tangent = vec3(model * vec4(aTangent, 0,0));
	Bitangent = vec3(model * vec4(aBitangent, 0,0));
	// gl_Position = projection * view * model * vec4(aPos, 1.0f) + vec4(0.1*aNormal, 1.0f);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}