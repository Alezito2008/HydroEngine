#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	vec3 albedo = texture(texture_diffuse1, fs_in.TexCoords).rgb;

	if (length(albedo) < 0.01)
		albedo = vec3(0.8);

	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specularMap = texture(texture_specular1, fs_in.TexCoords).rgb;
	float specularStrength = max(max(specularMap.r, specularMap.g), specularMap.b);
	if (specularStrength == 0.0)
		specularStrength = 0.5;
	vec3 specular = specularStrength * spec * lightColor;

	vec3 color = (ambient + diffuse) * albedo + specular;
	FragColor = vec4(color, 1.0);
}
