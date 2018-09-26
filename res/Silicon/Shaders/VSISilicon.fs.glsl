//#version 430 core
//
//out vec4 color;
//
//in VS_OUT
//{ 
//    
//    vec2 texCoord;
//    vec3 color;
//} fs_in;
//
//uniform sampler2D tex;
//
//void main(void)
//{ 
//    color =	texture(tex, fs_in.texCoord);
//    color += vec4(fs_in.color, 1.0f);
//}

#version 430 core

layout (location = 0) out vec4 color;

in VS_OUT
{
	vec2 texCoord;
	vec3 N;
	vec3 L;
	vec3 V;
}fs_in;

uniform sampler2D tex;

uniform vec3 diffuse_albedo = vec3(0.5, 0.5, 0.6);
uniform vec3 specular_albedo = vec3(0.1, 0.1, 0.6);
uniform float specular_power = 500.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.6);
uniform float alpha_value = 0.35f;

void main(void)
{
	vec4 text = texture2D(tex, fs_in.texCoord);

	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	vec3 R = reflect(-L, N);

	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
	vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

	color = vec4(ambient + diffuse + specular, alpha_value);

	//vec3 col = mix(color.rgb, text.rgb, alpha_value);
    //
	//color *= vec4(col,alpha_value);
}