#version 450 core

in vec3 texcoord;
out vec4 fragColor;

uniform samplerCube cubeSampler;

void main()
{
   fragColor = texture(cubeSampler, texcoord);
}
