#include "cRenderModel.h"
#include <glad/glad.h>

#include "cTextureManager.h"

cRenderModel::cRenderModel()
{
	position = glm::vec3(0.f);
	orientation = glm::vec3(0.f);
	scale = glm::vec3(1.f);

	isWireframe = false;
	isInstanced = false;
	useWholeColor = false;

	wholeColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

	meshName = "";
	textureName = "";

	shaderName = "scene";
}

cRenderModel::~cRenderModel()
{
}

void cRenderModel::InstanceObject(std::vector<glm::vec4>& offsets, unsigned int program)
{
	isInstanced = true;
	instancedNum = offsets.size();

	// Generate offsets buffer
	glGenBuffers(1, &(instanceOffsetsBufferId));

	glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetsBufferId);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * offsets.size(),
		(GLvoid*)&offsets[0],
		GL_STATIC_DRAW);

	GLint offset_location = glGetAttribLocation(program, "oOffset");
	glEnableVertexAttribArray(offset_location);
	glVertexAttribPointer(offset_location, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(glm::vec4),
		(void*)0);
	glVertexAttribDivisor(offset_location, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(offset_location);
}

// This might change when dynamic map loading is implemented (not using a general texture map)
void cRenderModel::SetUpUniforms()
{
	if (textureName == "") return;

	cTextureManager::GetInstance()->SetupTexture(textureName);
}
