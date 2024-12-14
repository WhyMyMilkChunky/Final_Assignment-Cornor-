#version 460 core

out vec4 FragColor;

in vec3 normal;

void main()
{
    vec3 n = normalize(normal);
    FragColor = vec4(n, 1.0);
}
