#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoords;

out vec2 textureCoords;
out float flip;

uniform int charId[7];
uniform int posX[7];
uniform int posY[7];
uniform int sizeX[7];
uniform int sizeY[7];

uniform vec2 originOffset;
uniform int glyphSize;
uniform float glyphPixelRatio;
uniform int screenWidth;
uniform int screenHeight;

uniform int atlasRowsNum;
uniform int atlasColsNum;

void main()
{    
    // Move model so bottom left vertex is at origin (0,0) and is 1 width and height
    vec3 newPos;
    newPos.x = (aPos.x + 1.f) / 2.f;
    newPos.y = (aPos.y + 1.f) / 2.f;

    newPos.x *= sizeX[gl_InstanceID] * glyphPixelRatio / float(screenWidth);
    newPos.y *= sizeY[gl_InstanceID] * glyphPixelRatio / float(screenHeight);

    float posTranslateX = posX[gl_InstanceID] * glyphPixelRatio * 2 / float(screenWidth) + originOffset.x;
    float posTranslateY = originOffset.y - (posY[gl_InstanceID] * glyphPixelRatio * 2 / float(screenHeight));

    newPos.x *= 2.f;
    newPos.y *= 2.f;
    newPos.x += posTranslateX;
    newPos.y += posTranslateY;

    vec2 tileSize = vec2(1.f / float(atlasColsNum), 1.f / float(atlasRowsNum));
    float uvTranslateX = charId[gl_InstanceID] % atlasColsNum * tileSize.x;
    float uvTranslateY = charId[gl_InstanceID] / atlasRowsNum * tileSize.y;

    textureCoords.x = aTextureCoords.x * tileSize.x * (sizeX[gl_InstanceID] / float(glyphSize));
    textureCoords.y = aTextureCoords.y * tileSize.y * (sizeY[gl_InstanceID] / float(glyphSize));

    textureCoords.x += uvTranslateX;
    textureCoords.y += uvTranslateY;
    
    flip = 0.f;
    if (gl_InstanceID % 2 == 0) {
        flip = 1.f;
    }

    gl_Position = vec4(newPos, 1.0);
}