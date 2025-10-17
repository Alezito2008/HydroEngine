



#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform MVP mvp;

mat3 computeNormalMatrix(mat4 modelMatrix)
{
	return transpose(inverse(mat3(modelMatrix)));
}

void main()
{
	vec4 worldPos = mvp.model * vec4(aPos, 1.0);
	vs_out.FragPos = worldPos.xyz;
	vs_out.Normal = computeNormalMatrix(mvp.model) * aNormal;
	vs_out.TexCoords = aTexCoords;
	gl_Position = mvp.projection * mvp.view * worldPos;
}
















