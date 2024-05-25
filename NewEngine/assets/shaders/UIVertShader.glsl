#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoords;

out vec2 textureCoords;

uniform float widthPercent;
uniform float heightPercent;

uniform float widthTranslate;
uniform float heightTranslate;

void main()
{    
    vec3 newPos;
    newPos.x = aPos.x * widthPercent;
    newPos.y = aPos.y * heightPercent;

    newPos.x += widthTranslate;
    newPos.y += heightTranslate;

    textureCoords = aTextureCoords;
    gl_Position = vec4(newPos, 1.0);
}