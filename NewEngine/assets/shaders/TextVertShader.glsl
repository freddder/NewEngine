#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoords;
layout (location = 2) in vec4 charData;
layout (location = 3) in float charId;

out vec2 textureCoords;

uniform int screenWidth;
uniform int screenHeight;

uniform vec2 originOffset;

uniform int glyphSize;
uniform float glyphPixelRatio;
uniform int atlasRowsNum;
uniform int atlasColsNum;

void main()
{    
    // Move model so bottom left vertex is at origin (0,0) and is 1 width and height
    vec3 newPos;
    newPos.x = (aPos.x + 1.f) / 2.f;
    newPos.y = (aPos.y + 1.f) / 2.f;

    float charPosX = charData.x;
    float charPosY = charData.y;
    float charSizeX = charData.z;
    float charSizeY = charData.a;
    int charValue = int(charId) - 33;

    newPos.x *= charSizeX * glyphPixelRatio / float(screenWidth);
    newPos.y *= charSizeY * glyphPixelRatio / float(screenHeight);

    float posTranslateX = originOffset.x + (charPosX * glyphPixelRatio / float(screenWidth) * 2);
    float posTranslateY = originOffset.y - (charPosY * glyphPixelRatio / float(screenHeight) * 2);

    newPos.x *= 2.f;
    newPos.y *= 2.f;
    newPos.x += posTranslateX;
    newPos.y += posTranslateY;

    vec2 tileSize = vec2(1.f / float(atlasColsNum), 1.f / float(atlasRowsNum));
    float uvTranslateX = charValue % atlasColsNum * tileSize.x;
    float uvTranslateY = charValue / atlasColsNum * tileSize.y;

    textureCoords.x = aTextureCoords.x * tileSize.x * (charSizeX / float(glyphSize));
    textureCoords.y = aTextureCoords.y * tileSize.y * (charSizeY / float(glyphSize));

    textureCoords.x += uvTranslateX;
    textureCoords.y += uvTranslateY;

    gl_Position = vec4(newPos, 1.0);
}