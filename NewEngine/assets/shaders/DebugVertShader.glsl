#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//uniform float scrWidth;
//uniform float scrHeight;
//
//uniform float desiredHeightPercent;
//uniform float desiredRatio;

uniform float widthPercent;
uniform float heightPercent;

void main()
{
//    float heightPercent = desiredHeightPercent;
//
//    float pixelHeight = scrHeight * desiredHeightPercent;
//    float pixelWidth = pixelHeight / desiredRatio;
//
//    float widthPercent = pixelWidth / scrWidth;
//
//    vec3 newPos;
//    newPos.x = aPos.x;// * widthPercent;
//    newPos.y = aPos.y * heightPercent;
    
    vec3 newPos;
    newPos.x = aPos.x * widthPercent;
    newPos.y = aPos.y * heightPercent;

    TexCoords = aTexCoords;
    gl_Position = vec4(newPos, 1.0);
}