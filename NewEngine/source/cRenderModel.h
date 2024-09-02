#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class cRenderModel
{
public:

	cRenderModel();
	~cRenderModel();

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

	void InstanceObject(std::vector<glm::vec4>& offsets);

	virtual void SetUpUniforms();
};