#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoords;

out vec2 textureCoords;

uniform float widthPercent;
uniform float heightPercent;

uniform float widthTranslate;
uniform float heightTranslate;

uniform int charId;
uniform int atlasRowsNum;
uniform int atlasColsNum;

void main()
{    
    //vec2 originOffset = vec2(oCharData1.x, oCharData1.y);
    //float widthPercent = oCharData1.z;
    //float heightPercent = oCharData1.a;

    // Move model so bottom left vertex is at origin (0,0)
    vec3 newPos;
    newPos.x = (aPos.x + 1.f) / 2.f * widthPercent;
    newPos.y = (aPos.y + 1.f) / 2.f * heightPercent;

    newPos.x += widthTranslate;  //originOffset.x;
    newPos.y += heightTranslate; //originOffset.y;

    vec2 tileSize = vec2(1.f / float(atlasColsNum), 1.f / float(atlasRowsNum));
    float finalX = (aTextureCoords.x * tileSize.x) + (charId % atlasColsNum * tileSize.x);
    float finalY = (aTextureCoords.y * tileSize.y) + (charId / atlasRowsNum * tileSize.y);
    textureCoords = vec2(finalX, finalY);

    gl_Position = vec4(newPos, 1.0);
}