#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

//static const unsigned int MAX_TEXTURES = 5;

//enum eAnimationType
//{
//	None,
//	Sprite,
//	UVShifting
//};

class cRenderModel
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

	std::string shaderName;
	//eAnimationType textureAnimationType;
	//int currSpriteId;
	//bool useGlobalPosForUV;
	//glm::vec2 globalUVRatios;
	//glm::vec3 textureOffset;

	cRenderModel();

	void InstanceObject(std::vector<glm::vec4>& offsets, unsigned int program);

	virtual void SetUpUniforms();

	//std::string textureNames[MAX_TEXTURES];
	//float textureRatios[MAX_TEXTURES];
};