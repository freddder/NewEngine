#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

//static const unsigned int MAX_TEXTURES = 5;

enum eAnimationType
{
	None,
	Sprite,
	UVShifting
};

class cModel
{
public:

	std::string meshName;

	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 scale;

	bool isWireframe;

	bool isInstanced;
	unsigned int instanceOffsetsBufferId;
	unsigned int instancedNum;

	bool useWholeColor;
	glm::vec4 wholeColor;

	std::string textureName;

	eAnimationType textureAnimationType;
	int currSpriteId;
	glm::vec2 textureOffset;

	cModel();

	void InstanceObject(std::vector<glm::vec4>& offsets, unsigned int program);

	//std::string textureNames[MAX_TEXTURES];
	//float textureRatios[MAX_TEXTURES];
};