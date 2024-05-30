#version 330 core
in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D texture_0;
uniform vec3 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_0, textureCoords).r);

    if (sampled.a < 0.1)
		discard;

    FragColor = vec4(color, 1.0) * sampled;
}