#version 450

layout(location = 0) in vec3 vpos;
layout(location = 2) in vec3 vnormal;
layout(location = 3) in vec3 vtexcoord;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec4 worldPos;
out vec3 normal, texcoord;

void main()
{
   texcoord = vtexcoord;
   texcoord.y = 1.0f - texcoord.y;
   
   normal = mat3(modelMatrix) * vnormal;
   worldPos = modelMatrix * vec4(vpos, 1.0f);

   gl_Position = projectionViewMatrix * worldPos;
}
