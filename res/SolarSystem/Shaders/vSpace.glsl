#version 450 core

layout(location = 0) in vec3 vpos;
layout(location = 3) in vec2 vtexcoord;

uniform mat4 projectionViewMatrix;

out vec3 texcoord;

void main()
{
   texcoord = vpos;
   texcoord.x *= -1.0f;

   gl_Position = projectionViewMatrix * vec4(vpos, 1.0f);
}
