#version 330 core
out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D texture_0;;

void main()
{
    vec4 color = texture(texture_0, textureCoords);

    if (color.a < 0.1)
			discard;

    FragColor = color; // orthographic
}