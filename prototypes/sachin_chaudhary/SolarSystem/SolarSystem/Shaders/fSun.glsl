#version 450 core

in vec3 texcoord;
out vec4 fragColor;

uniform sampler2D sampler;

void main()
{
   fragColor = texture2D(sampler, texcoord.xy);
}
