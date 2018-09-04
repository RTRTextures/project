#version 450 core

in vec4 worldPos;
in vec3 normal, texcoord;
out vec4 fragColor;

uniform vec3 lightLocationWorld;
uniform sampler2D sampler;

void main()
{
   vec3 l = normalize(-worldPos.xyz);
   vec3 n = normalize(normal);

   float diffuse = max(dot(l, n), 0.0f);

   fragColor = diffuse * texture2D(sampler, texcoord.xy);
}
