#version 450

layout(location = 0) in vec3 vpos;
layout(location = 3) in vec3 vtexcoord;

uniform mat4 mvp;

out vec3 texcoord;

void main()
{
   texcoord = vtexcoord;
   texcoord.y = 1.0f - texcoord.y;
   

   gl_Position = mvp * vec4(vpos, 1.0f);
}
