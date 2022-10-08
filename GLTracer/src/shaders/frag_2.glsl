#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 
in vec2 TexCoord;

uniform float mixValue;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
    vec3 result = LightingColor * texColor.rgb;
    FragColor = vec4(result, 1);
}