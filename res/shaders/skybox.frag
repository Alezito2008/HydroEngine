#version 330 core

in vec3 vDirection;

out vec4 FragColor;

uniform sampler2D skyboxTexture;

const float PI = 3.1415926535897932384626433832795;

vec2 SampleSphericalMap(vec3 dir)
{
    vec3 d = normalize(dir);
    float u = atan(d.z, d.x) / (2.0 * PI) + 0.5;
    float v = 0.5 - asin(clamp(d.y, -1.0, 1.0)) / PI;
    return vec2(u, v);
}

void main()
{
    vec2 uv = SampleSphericalMap(vDirection);
    FragColor = texture(skyboxTexture, uv);
}
